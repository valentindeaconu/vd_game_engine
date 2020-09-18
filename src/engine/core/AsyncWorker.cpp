//
// Created by Vali on 9/16/2020.
//

#include "AsyncWorker.hpp"

namespace vd::core {
    AsyncWorker::AsyncWorker(AsyncRoutine routine)
        : asyncRoutine(std::move(routine))
        , alive(true)
        , execute(false)
        , processed(false)
        , worker([&]() {
                while (alive) {
                    std::unique_lock<std::mutex> lk(m);
                    cv.wait(lk, [&]() { return execute; });

                    execute = false;

                    asyncRoutine();

                    processed = true;

                    lk.unlock();
                    cv.notify_one();
                }
            })
    {
    }

    AsyncWorker::~AsyncWorker() {
        alive = false;
        worker.join();
    }

    void AsyncWorker::run() {
        {
            std::lock_guard<std::mutex> lk(m);
            execute = true;
        }
        cv.notify_one();
    }

    void AsyncWorker::join() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]() { return processed; });
        lk.unlock();
    }

    void AsyncWorker::stop() {
        alive = false;
        worker.join();
    }
}

