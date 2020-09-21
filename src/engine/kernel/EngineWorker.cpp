#include "EngineWorker.hpp"

namespace vd::kernel {
    void Observable::subscribe(const ObserverPtr& observer) {
        core::AsyncWorkerPtr workerPtr = std::make_shared<core::AsyncWorker>([&]() {
            observer->update();
        });
        observers.emplace_back(observer, std::move(workerPtr));
    }

    void Observable::unsubscribe(const ObserverPtr& observer) {
        auto it = std::find_if(observers.begin(), observers.end(), [&](const ObserverWorkerPair & x) {
            return x.first == observer;
        });

        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void Observable::broadcastInit() {
        for (auto& pair : observers) {
            pair.first->init();
        }
    }

    void Observable::broadcastUpdate() {
        /*for (const auto& pair : observers) {
            pair.second->run();
        }

        for (const auto& pair : observers) {
            pair.second->join();
        }*/

        for (auto& pair : observers) {
            pair.first->update();
        }
    }

    void Observable::broadcastRender(const RenderingPass& renderingPass) {
        for (auto& pair : observers) {
            pair.first->render(renderingPass);
        }
    }

    void Observable::broadcastCleanUp() {
        for (auto& pair : observers) {
            pair.first->cleanUp();
        }
    }

    EngineWorker::EngineWorker() = default;

    EngineWorker::~EngineWorker() = default;

    void EngineWorker::init() {
        broadcastInit();
    }

    void EngineWorker::update() {
        broadcastUpdate();
    }

    void EngineWorker::render(const RenderingPass& renderingPass) {
        broadcastRender(renderingPass);
    }

    void EngineWorker::cleanUp() {
        broadcastCleanUp();
    }
}