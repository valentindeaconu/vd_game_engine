//
// Created by Vali on 2/24/2021.
//

#include "SingularInputStage.hpp"

namespace vd::postprocessing {

    SingularInputStage::SingularInputStage(std::string name,
                                           float scale,
                                           FrameBufferResolver inputResolver,
                                           FrameBufferConfigurator frameBufferConfigurator,
                                           Predicate precondition,
                                           Consumer beforeExecution,
                                           Consumer afterExecution)
        : vd::postprocessing::Stage(std::move(name))
        , m_Scale(scale)
        , m_fnResolver(std::move(inputResolver))
        , m_fnConfigurator(std::move(frameBufferConfigurator))
        , m_fnPrecondition(std::move(precondition))
        , m_fnBeforeExec(std::move(beforeExecution))
        , m_fnAfterExec(std::move(afterExecution))
    {
    }

    void SingularInputStage::Setup(vd::Dimension dimension) {
        m_pInputFrameBuffer = m_fnResolver();

        auto pFrameBuffer = std::make_shared<vd::gl::FrameBuffer>(dimension.width * m_Scale, dimension.height * m_Scale);

        m_fnConfigurator(pFrameBuffer);

        FrameBuffer() = std::move(pFrameBuffer);
    }

    bool SingularInputStage::Precondition() {
        return m_fnPrecondition();
    }

    void SingularInputStage::Prepare() {
        m_fnBeforeExec();
    }

    void SingularInputStage::Finish() {
        m_fnAfterExec();
    }

    vd::gl::FrameBufferPtr& SingularInputStage::InputFrameBuffer() {
        return m_pInputFrameBuffer;
    }


}