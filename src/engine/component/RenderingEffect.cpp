//
// Created by Vali on 2/19/2021.
//

#include "RenderingEffect.hpp"

namespace vd::component {

    IRenderingEffect::IRenderingEffect(std::string name)
        : m_Name(std::move(name))
    {
    }

    const std::string& IRenderingEffect::Name() {
        return m_Name;
    }

    gl::FrameBufferPtr& IRenderingEffect::FrameBuffer() {
        return m_pFrameBuffer;
    }

    ConcreteEffect::ConcreteEffect(std::string name,
                                   float scale,
                                   FrameBuffersResolver inputGetter,
                                   FrameBufferConfigurator frameBufferConfigurator,
                                   Predicate precondition,
                                   Consumer beforeExecution,
                                   Consumer afterExecution)
            : IRenderingEffect(std::move(name))
            , m_Scale(scale)
            , m_fnResolver(std::move(inputGetter))
            , m_fnConfigurator(std::move(frameBufferConfigurator))
            , m_fnPrecondition(std::move(precondition))
            , m_fnBeforeExec(std::move(beforeExecution))
            , m_fnAfterExec(std::move(afterExecution))
    {
    }

    void ConcreteEffect::Init(vd::Dimension dimension) {
        m_pInputFrameBuffers = m_fnResolver();

        auto pFrameBuffer = std::make_shared<vd::gl::FrameBuffer>(dimension.width * m_Scale, dimension.height * m_Scale);
        pFrameBuffer->Create();

        m_fnConfigurator(pFrameBuffer);

        FrameBuffer() = std::move(pFrameBuffer);
    }

    void ConcreteEffect::CleanUp() {
        FrameBuffer()->CleanUp();
    }

    bool ConcreteEffect::Precondition() {
        return m_fnPrecondition();
    }

    void ConcreteEffect::Prepare() {
        m_fnBeforeExec();
        FrameBuffer()->Bind();
        FrameBuffer()->Clear();
    }

    void ConcreteEffect::Finish() {
        FrameBuffer()->Unbind();
        m_fnAfterExec();
    }

    vd::gl::FrameBufferPtr& ConcreteEffect::InputFrameBuffer(size_t index) {
        return m_pInputFrameBuffers[index];
    }

}