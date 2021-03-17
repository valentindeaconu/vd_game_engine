//
// Created by Vali on 11/18/2020.
//

#include "Context.hpp"

namespace vd::context {

    Context::Context()
        : m_WireframeMode(false)
        , m_FPS(0)
        , m_FrameTimeInSeconds(0.0f)
        , m_SamplesPerPixel(1)
        , m_ClipPlane(0.0f)
        , m_pSceneFrameBuffer(nullptr)
    {
    }

    bool& Context::WireframeMode() {
        return m_WireframeMode;
    }

    int& Context::FPS() {
        return m_FPS;
    }

    int& Context::SamplesPerPixel() {
        return m_SamplesPerPixel;
    }

    float& Context::FrameTime() {
        return m_FrameTimeInSeconds;
    }

    glm::vec4& Context::ClipPlane() {
        return m_ClipPlane;
    }

    gl::FrameBufferPtr& Context::SceneFrameBuffer() {
        return m_pSceneFrameBuffer;
    }

    ContextManager::ContextManager() {
        m_pContext = std::make_shared<Context>();

        vd::ObjectOfType<Context>::Provide(m_pContext);
    }

    void ContextManager::Link() {
        m_pEventHandler = vd::ObjectOfType<event::EventHandler>::Find();
    }

    void ContextManager::Init() {

    }

    void ContextManager::Update() {
        if (m_pEventHandler->KeyDown(GLFW_KEY_M)) {
            m_pContext->m_WireframeMode = !m_pContext->m_WireframeMode;
        }
    }

    void ContextManager::CleanUp() {

    }
}
