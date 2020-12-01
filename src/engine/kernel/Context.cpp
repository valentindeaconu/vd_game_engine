//
// Created by Vali on 11/18/2020.
//

#include "Context.hpp"

namespace vd::kernel {

    Context::Context()
        : m_FPS(0)
        , m_FrameTimeInSeconds(0.0f)
        , m_ClipPlane(0.0f)
    {
    }

    Context::~Context() = default;

    int& Context::FPS() {
        return m_FPS;
    }

    float& Context::FrameTime() {
        return m_FrameTimeInSeconds;
    }

    glm::vec4& Context::ClipPlane() {
        return m_ClipPlane;
    }

}
