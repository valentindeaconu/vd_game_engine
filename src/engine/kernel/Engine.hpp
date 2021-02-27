//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_ENGINE_HPP
#define VD_GAME_ENGINE_ENGINE_HPP

#include <engine/api/gl/GL.hpp>

#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <engine/datastruct/Observer.hpp>
#include <engine/component/RenderingPass.hpp>

#include <engine/misc/Types.hpp>

#include <engine/window/Window.hpp>
#include <engine/context/Context.hpp>

namespace vd {
    class Engine : public datastruct::Observable {
    public:
        Engine();
        ~Engine();

        void Link();

        void Init();
        void Start();

        void Add(const component::RenderingPass& renderingPass);
        void Remove(const std::string& name);
    private:
        void Summary();

        void Run();
        void Stop();

        void Update();
        void Render();
        void CleanUp();

        float   m_FrameTime;
        bool    m_Running;

        window::WindowPtr   m_pWindow;
        context::ContextPtr  m_pContext;
        std::list<component::RenderingPass>         m_RenderingPasses;
    };
    typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif //VD_GAME_ENGINE_ENGINE_HPP