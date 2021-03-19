//
// Created by Vali on 3/8/2021.
//

#include "Job.hpp"

namespace vd::core {

    Job::Job(Consumer action, bool autoRun)
        : m_Action(std::move(action))
        , m_OnFinish(vd::g_kEmptyConsumer)
        , m_Done(false)
        , m_Running(autoRun)
    {
    }

    void Job::DoJob() {
        // lock scope (wait for run signal)
        {
            std::unique_lock<std::mutex> lock(m_RLock);
            while (!m_Running) {
                m_RCondVar.wait(lock);
            }
            lock.unlock();
        }

        m_Action();

        // lock scope (set done)
        {
            std::unique_lock<std::mutex> lock(m_DLock);
            m_Done = true;
            lock.unlock();

            m_DCondVar.notify_all();
        }

        // lock scope (set run signal)
        {
            std::unique_lock<std::mutex> lock(m_RLock);
            m_Running = false;
        }

        m_OnFinish();
    }

    void Job::Run() {
        if (m_Running) {
            return;
        }

        std::unique_lock<std::mutex> lock(m_RLock);
        m_Running = true;
        lock.unlock();

        m_RCondVar.notify_all();
    }

    void Job::Join() {
        std::unique_lock<std::mutex> lock(m_DLock);
        while (!m_Done) {
            m_DCondVar.wait(lock);
        }
    }

    void Job::Reset() {
        std::unique_lock<std::mutex> lock(m_DLock);
        if (m_Done) {
            m_Done = false;
        }
    }

    bool Job::Done() {
        std::unique_lock<std::mutex> lock(m_DLock);
        while (!m_Done) {
            m_DCondVar.wait(lock);
        }
        return true;
    }

    void Job::OnFinish(vd::Consumer action) {
        m_OnFinish = std::move(action);
    }
}

