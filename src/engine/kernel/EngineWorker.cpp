#include "EngineWorker.hpp"

namespace vd::kernel
{
    void Observable::subscribe(const ObserverPtr& observer)
    {
        this->observers.push_back(observer);
    }

    void Observable::unsubscribe(const ObserverPtr& observer)
    {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void Observable::broadcastInit()
    {
        for (auto& observer : observers)
        {
            observer->init();
        }
    }

    void Observable::broadcastUpdate(bool shadowUpdate)
    {
        for (auto& observer : observers)
        {
            observer->update(shadowUpdate);
        }
    }

    void Observable::broadcastCleanUp()
    {
        for (auto& observer : observers)
        {
            observer->cleanUp();
        }
    }

    EngineWorker::EngineWorker() = default;

    EngineWorker::~EngineWorker() = default;

    void EngineWorker::init()
    {
        broadcastInit();
    }

    void EngineWorker::update(bool shadowUpdate)
    {
        broadcastUpdate(shadowUpdate);
    }

    void EngineWorker::cleanUp()
    {
        broadcastCleanUp();
    }
}