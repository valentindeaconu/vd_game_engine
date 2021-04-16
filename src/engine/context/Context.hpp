//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_CONTEXT_HPP
#define VD_GAME_ENGINE_CONTEXT_HPP

#include <glm/glm.hpp>

#include <memory>

#include <engine/gapi/gl/FrameBuffer.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/event/EventHandler.hpp>

#include <engine/component/IManager.hpp>

namespace vd::context {
    class ContextManager;

    class Context {
    public:
        Context();

        bool& WireframeMode();
        int& FPS();
        int& SamplesPerPixel();
        float& FrameTime();
        glm::vec4& ClipPlane();
        gl::FrameBufferPtr& SceneFrameBuffer();
    private:
        friend class ContextManager;

        bool                    m_WireframeMode;
        int                     m_FPS;
        int                     m_SamplesPerPixel;
        float                   m_FrameTimeInSeconds;
        glm::vec4               m_ClipPlane;
        gl::FrameBufferPtr      m_pSceneFrameBuffer;
    };
    typedef std::shared_ptr<Context>  ContextPtr;

    class ContextManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        ContextManager();

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        ContextPtr              m_pContext;
        event::EventHandlerPtr  m_pEventHandler;
    };
    typedef std::shared_ptr<ContextManager>  ContextManagerPtr;
}

#endif //VD_GAME_ENGINE_CONTEXT_HPP
