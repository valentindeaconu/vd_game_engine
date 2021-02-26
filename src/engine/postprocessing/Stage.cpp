//
// Created by Vali on 2/19/2021.
//

#include "Stage.hpp"

namespace vd::postprocessing {

    Stage::Stage(std::string name)
        : m_Name(std::move(name))
    {
    }

    void Stage::Link() {
        m_pWindow = vd::ObjectOfType<window::Window>::Find();
    }

    void Stage::Init() {
        Setup(vd::Dimension(m_pWindow->Width(), m_pWindow->Height()));

        if (m_pFrameBuffer->Status() != gl::FrameBuffer::eComplete) {
            throw RuntimeError("FrameBuffer with id " + std::to_string(m_pFrameBuffer->Id()) +
                " has status: " +m_pFrameBuffer->StatusAsString());
        }
    }

    void Stage::CleanUp() {
        m_pFrameBuffer = nullptr;
    }

    const std::string& Stage::Name() {
        return m_Name;
    }

    gl::FrameBufferPtr& Stage::FrameBuffer() {
        return m_pFrameBuffer;
    }

}