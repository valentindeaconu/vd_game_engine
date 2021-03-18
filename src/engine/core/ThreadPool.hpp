//
// Created by Vali on 3/8/2021.
//

#ifndef VDGE_THREADPOOL_HPP
#define VDGE_THREADPOOL_HPP

#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>

#include <engine/logger/Logger.hpp>
#include <engine/exception/Exceptions.hpp>
#include <engine/defines/Types.hpp>

#include "Job.hpp"

namespace vd::core {
    namespace exception {
        struct ThreadError : public CustomException {
            explicit ThreadError(const std::string& message);
        };
    }

    class ThreadPool {
    public:
        explicit ThreadPool(const std::vector<std::string>& specializations = {});

        void Release();

        JobPtr  CreateJob(vd::Consumer action, bool autoRun = false);
        JobPtr  CreateJobFor(vd::Consumer action, const std::string& workerName, bool autoRun = false);
        void    PushJobFor(const JobPtr& pJob, const std::string& workerName);
    private:
        typedef uint16_t index_t;

        static void Routine(ThreadPool* pool, index_t workerIndex);

        struct Worker {
            std::thread         Thread;
            std::atomic_bool    Closing;
        };

        const index_t                   m_kMaxCoreCount;
        std::vector<Worker>             m_Workers;
        std::map<std::string, index_t>  m_NameToIndex;

        struct SyncQueue {
            std::queue<JobPtr>  Queue;
            std::mutex          Lock;
        };

        SyncQueue                       m_JobQueue;
        std::vector<SyncQueue>          m_SpecializedJobQueue;
    };
    typedef std::shared_ptr<ThreadPool>     ThreadPoolPtr;
}


#endif //VDGE_THREADPOOL_HPP
