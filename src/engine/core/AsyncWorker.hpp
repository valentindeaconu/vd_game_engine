//
// Created by Vali on 9/16/2020.
//

#ifndef VD_GAME_ENGINE_ASYNCWORKER_HPP
#define VD_GAME_ENGINE_ASYNCWORKER_HPP

#include <memory>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>

namespace vd::core {
    class AsyncWorker {
    public:
        typedef std::function<void()> AsyncRoutine;

        explicit AsyncWorker(AsyncRoutine routine);

        ~AsyncWorker();

        AsyncWorker(const AsyncWorker&) = delete;
        AsyncWorker(AsyncWorker&&) = delete;
        AsyncWorker& operator=(AsyncWorker) = delete;


        void run();
        void join();
        void stop();
    private:
        AsyncRoutine asyncRoutine;

        std::atomic<bool> alive;

        bool execute;
        bool processed;
        std::mutex m;
        std::condition_variable cv;

        std::thread worker;
    };
    typedef std::shared_ptr<AsyncWorker>    AsyncWorkerPtr;
}



#endif //VD_GAME_ENGINE_ASYNCWORKER_HPP
