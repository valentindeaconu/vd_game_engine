//
// Created by Vali on 3/8/2021.
//

#include "ThreadPool.hpp"

namespace vd::core {
    exception::ThreadError::ThreadError(const std::string &message)
        : CustomException("ThreadError", message)
    {
    }

    ThreadPool::ThreadPool(const std::vector<std::string>& specializations)
        : m_kMaxCoreCount(std::thread::hardware_concurrency())
        , m_Workers(m_kMaxCoreCount - 1)
        , m_SpecializedJobQueue(specializations.size())
    {
        if (m_kMaxCoreCount == 0) {
            throw exception::ThreadError("could not detect number of logical processors");
        }

        // 1 thread reserved, the main thread
        if (specializations.size() > m_kMaxCoreCount - 1) {
            vd::Logger::warn("There are too many specialized threads, this machine allows only " + std::to_string(m_kMaxCoreCount));
        }

        size_t i = 0;
        for (; i < specializations.size(); ++i) {
            auto& name = specializations[i];

            Worker& w = m_Workers[i];
            w.Thread = std::thread(Routine, this, i);
            w.Closing.store(false, std::memory_order_relaxed);

            m_NameToIndex[name] = i;
        }

        for (; i < m_kMaxCoreCount - 1; ++i) {
            Worker& w = m_Workers[i];

            w.Thread = std::thread(Routine, this, i);
            w.Closing.store(false, std::memory_order_relaxed);
        }
    }

    ThreadPool::~ThreadPool() {
        for (auto& w : m_Workers) {
            w.Closing.store(true, std::memory_order_relaxed);
        }

        for (auto& w : m_Workers) {
            if (w.Thread.joinable()) {
                w.Thread.join();
            }
        }
    }

    void ThreadPool::Routine(ThreadPool* pool, uint16_t workerIndex) {
        using namespace std::literals::chrono_literals;

        SyncQueue& syncQueue = (workerIndex < pool->m_SpecializedJobQueue.size())
                ? pool->m_SpecializedJobQueue[workerIndex] : pool->m_JobQueue;

        for(;;) {
            if (pool->m_Workers[workerIndex].Closing.load(std::memory_order_relaxed)) {
                break;
            }

            JobPtr job = nullptr;

            std::unique_lock<std::mutex> lock(syncQueue.Lock);
            if (!syncQueue.Queue.empty()) {
                job = syncQueue.Queue.front();
                syncQueue.Queue.pop();
            }
            lock.unlock();

            if (job != nullptr) {
                job->DoJob();
            }

            std::this_thread::sleep_for(1ms);
        }
    }

    JobPtr ThreadPool::CreateJob(vd::Consumer action) {
        JobPtr job = std::make_shared<Job>(std::move(action));

        std::unique_lock<std::mutex> lock(m_JobQueue.Lock);
        m_JobQueue.Queue.push(job);

        return job;
    }

    JobPtr ThreadPool::CreateJobFor(vd::Consumer action, const std::string& workerName) {
        if (m_NameToIndex.contains(workerName)) {
            JobPtr job = std::make_shared<Job>(std::move(action));

            index_t workerIndex = m_NameToIndex[workerName];

            std::unique_lock<std::mutex> lock(m_SpecializedJobQueue[workerIndex].Lock);
            m_SpecializedJobQueue[workerIndex].Queue.push(job);

            return job;
        } else {
            throw exception::ThreadError(workerName + " not registered as a specialized worker");
        }
    }

    void ThreadPool::PushJobFor(const JobPtr& pJob, const std::string& workerName) {
        if (m_NameToIndex.contains(workerName)) {
            index_t workerIndex = m_NameToIndex[workerName];

            std::unique_lock<std::mutex> lock(m_SpecializedJobQueue[workerIndex].Lock);
            m_SpecializedJobQueue[workerIndex].Queue.push(pJob);
        } else {
            throw exception::ThreadError(workerName + " not registered as a specialized worker");
        }
    }

}
