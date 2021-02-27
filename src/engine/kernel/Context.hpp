//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_CONTEXT_HPP
#define VD_GAME_ENGINE_CONTEXT_HPP

#include <glm/glm.hpp>

#include <memory>

#include <engine/api/gl/FrameBuffer.hpp>

namespace vd::kernel {
    class Context {
    public:
        Context();
        ~Context();

        int& FPS();
        float& FrameTime();
        glm::vec4& ClipPlane();
        gl::FrameBufferPtr& SceneFBO();
    private:
        int                     m_FPS;
        float                   m_FrameTimeInSeconds;
        glm::vec4               m_ClipPlane;
        gl::FrameBufferPtr      m_pSceneFBO;
    };
    typedef std::shared_ptr<Context>  ContextPtr;
}

#endif //VD_GAME_ENGINE_CONTEXT_HPP
