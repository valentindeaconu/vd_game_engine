//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_SINGULARINPUTSTAGE_HPP
#define VDGE_SINGULARINPUTSTAGE_HPP

#include "Stage.hpp"
#include "Types.hpp"

namespace vd::postprocessing {
    class SingularInputStage : public vd::postprocessing::Stage {
    public:
        explicit SingularInputStage(std::string name,
                                    float scale,
                                    FrameBufferResolver inputGetter,
                                    FrameBufferConfigurator frameBufferConfigurator,
                                    Predicate precondition = vd::g_kEmptyPredicate,
                                    Consumer beforeExecution = vd::g_kEmptyConsumer,
                                    Consumer afterExecution = vd::g_kEmptyConsumer);

        void Setup(vd::Dimension dimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFrameBuffer();
    private:
        float                       m_Scale;
        FrameBufferResolver         m_fnResolver;
        FrameBufferConfigurator     m_fnConfigurator;
        Predicate                   m_fnPrecondition;
        Consumer                    m_fnBeforeExec;
        Consumer                    m_fnAfterExec;
        vd::gl::FrameBufferPtr      m_pInputFrameBuffer;
    };
    typedef std::shared_ptr<SingularInputStage>   SingularInputStagePtr;
}

#endif //VDGE_SINGULARINPUTSTAGE_HPP
