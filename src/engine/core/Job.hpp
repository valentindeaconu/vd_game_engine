//
// Created by Vali on 3/8/2021.
//

#ifndef VDGE_JOB_HPP
#define VDGE_JOB_HPP

#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <engine/misc/Types.hpp>

namespace vd::core {

    class Job {
    public:
        explicit Job(vd::Consumer action = vd::g_kEmptyConsumer, bool autoRun = false);

        void DoJob();

        void Run();
        void Join();
        void Reset();

        [[nodiscard]] bool Done();

        void OnFinish(vd::Consumer action);
    private:
        vd::Consumer                m_Action;
        vd::Consumer                m_OnFinish;

        bool                        m_Done;
        std::mutex                  m_DLock;
        std::condition_variable     m_DCondVar;

        bool                        m_Running;
        std::mutex                  m_RLock;
        std::condition_variable     m_RCondVar;
    };
    typedef std::shared_ptr<Job>    JobPtr;

}


#endif //VDGE_JOB_HPP
