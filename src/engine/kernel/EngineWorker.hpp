#ifndef __ENGINE_WORKER_HPP_
#define __ENGINE_WORKER_HPP_

#include <memory>
#include <vector>
#include <algorithm>

#include <engine/core/AsyncWorker.hpp>

namespace vd::kernel {
    enum RenderingPass {
        eMain = 0,
        eReflection,
        eRefraction,
        eShadow
    };

    class Observer
    {
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Render(const RenderingPass& renderingPass) = 0;
        virtual void CleanUp() = 0;
    };
    typedef std::shared_ptr<Observer>	ObserverPtr;

    class Observable
    {
    public:
        void subscribe(const ObserverPtr& observer);
        void unsubscribe(const ObserverPtr& observer);

        void broadcastInit();
        void broadcastUpdate();
        void broadcastRender(const RenderingPass& renderingPass);
        void broadcastCleanUp();
    private:
        typedef std::pair<ObserverPtr, core::AsyncWorkerPtr>    ObserverWorkerPair;
        typedef std::vector<ObserverWorkerPair>                 ObserverWorkerPairVec;
        ObserverWorkerPairVec observers;
    };

    class EngineWorker : public Observable
    {
    public:
        EngineWorker();
        ~EngineWorker();

        void init();
        void update();
        void render(const RenderingPass& renderingPass);
        void cleanUp();
    };
    typedef std::shared_ptr<EngineWorker>	EngineWorkerPtr;
}

#endif // !__ENGINE_WORKER_HPP_