//
// Created by Vali on 11/12/2020.
//

#include "RenderingPass.hpp"

namespace vd::component {

    RenderingPass::RenderingPass(std::string name,
                                 const RenderingPass::priority_t& priority,
                                 gl::FrameBufferPtr frameBuffer,
                                 vd::Predicate precondition,
                                 vd::Consumer beforeExecution,
                                 vd::Consumer afterExecution)
        : m_Name(std::move(name))
        , m_Priority(priority)
        , m_pFrameBuffer(std::move(frameBuffer))
        , m_ClearEnabled(true)
        , m_Precondition(std::move(precondition))
        , m_BeforeExecution(std::move(beforeExecution))
        , m_AfterExecution(std::move(afterExecution))
    {
    }

    RenderingPass::~RenderingPass() = default;

    bool RenderingPass::Precondition() const {
        return m_Precondition();
    }

    void RenderingPass::Prepare() {
        m_BeforeExecution();

        if (m_pFrameBuffer != nullptr) {
            m_pFrameBuffer->Bind();
            if (m_ClearEnabled) {
                m_pFrameBuffer->Clear();
            }
        }
    }

    void RenderingPass::Finish() {
        if (m_pFrameBuffer != nullptr) {
            m_pFrameBuffer->Unbind();
        }
        m_AfterExecution();
    }

    const std::string& RenderingPass::Name() const {
        return m_Name;
    }

    const RenderingPass::priority_t& RenderingPass::Priority() const {
        return m_Priority;
    }

    const gl::FrameBufferPtr& RenderingPass::FrameBuffer() const {
        return m_pFrameBuffer;
    }

    bool& RenderingPass::Clearing() {
        return m_ClearEnabled;
    }

}