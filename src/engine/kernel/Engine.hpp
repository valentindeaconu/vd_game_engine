//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_ENGINE_HPP
#define VD_GAME_ENGINE_ENGINE_HPP

#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <engine/gapi/gl/Context.hpp>
#include <engine/datastruct/Observer.hpp>
#include <engine/component/RenderingPass.hpp>

#include <engine/defines/Types.hpp>

#include <engine/window/Window.hpp>
#include <engine/context/Context.hpp>

#include <engine/core/ThreadPool.hpp>

namespace vd {
    class Engine : public datastruct::Observable {
    public:
        Engine();

        void Link();

        void Prepare();
        void Start();

        void Add(const component::RenderingPass& renderingPass);
        void Remove(const std::string& name);
    private:
        void Summary();

        void Run();
        void Stop();

        void Init();
        void Update();
        void Render();
        void CleanUp();

        core::JobPtr                                m_pUpdateJob;
        core::JobPtr                                m_pRenderJob;

        float                                       m_FrameTime;
        bool                                        m_Running;
        std::atomic_int                             m_Frames;

        core::ThreadPoolPtr                         m_pThreadPool;
        window::WindowPtr                           m_pWindow;
        context::ContextPtr                         m_pContext;
        std::list<component::RenderingPass>         m_RenderingPasses;
    };
    typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif //VD_GAME_ENGINE_ENGINE_HPP