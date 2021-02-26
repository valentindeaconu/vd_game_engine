//
// Created by Vali on 2/24/2021.
//

#include "MultipleInputStage.hpp"

namespace vd::postprocessing {

    MultipleInputStage::MultipleInputStage(std::string name,
                                           float scale,
                                           FrameBuffersResolver inputGetter,
                                           FrameBufferConfigurator frameBufferConfigurator,
                                           Predicate precondition,
                                           Consumer beforeExecution,
                                           Consumer afterExecution)
        : vd::postprocessing::Stage(std::move(name))
        , m_Scale(scale)
        , m_fnResolver(std::move(inputGetter))
        , m_fnConfigurator(std::move(frameBufferConfigurator))
        , m_fnPrecondition(std::move(precondition))
        , m_fnBeforeExec(std::move(beforeExecution))
        , m_fnAfterExec(std::move(afterExecution))
    {
    }

    void MultipleInputStage::Setup(vd::Dimension dimension) {
        m_pInputFrameBuffers = m_fnResolver();

        auto pFrameBuffer = std::make_shared<vd::gl::FrameBuffer>(dimension.width * m_Scale, dimension.height * m_Scale);

        m_fnConfigurator(pFrameBuffer);

        FrameBuffer() = std::move(pFrameBuffer);
    }

    bool MultipleInputStage::Precondition() {
        return m_fnPrecondition();
    }

    void MultipleInputStage::Prepare() {
        m_fnBeforeExec();
    }

    void MultipleInputStage::Finish() {
        m_fnAfterExec();
    }

    vd::gl::FrameBufferPtr& MultipleInputStage::InputFrameBuffer(size_t index) {
        return m_pInputFrameBuffers[index];
    }
}