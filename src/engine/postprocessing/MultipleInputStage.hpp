//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_MULTIPLEINPUTSTAGE_HPP
#define VDGE_MULTIPLEINPUTSTAGE_HPP

#include "Stage.hpp"
#include "Types.hpp"

namespace vd::postprocessing {
    class MultipleInputStage : public vd::postprocessing::Stage {
    public:
        explicit MultipleInputStage(std::string name,
                                    float scale,
                                    FrameBuffersResolver inputGetter,
                                    FrameBufferConfigurator frameBufferConfigurator,
                                    Predicate precondition = vd::g_kEmptyPredicate,
                                    Consumer beforeExecution = vd::g_kEmptyConsumer,
                                    Consumer afterExecution = vd::g_kEmptyConsumer);

        void Setup(vd::Dimension dimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFrameBuffer(size_t index);
    private:
        float                       m_Scale;
        FrameBuffersResolver        m_fnResolver;
        FrameBufferConfigurator     m_fnConfigurator;
        Predicate                   m_fnPrecondition;
        Consumer                    m_fnBeforeExec;
        Consumer                    m_fnAfterExec;
        vd::gl::FrameBufferPtrVec   m_pInputFrameBuffers;
    };
    typedef std::shared_ptr<MultipleInputStage>   MultipleInputStagePtr;
}


#endif //VDGE_MULTIPLEINPUTSTAGE_HPP
