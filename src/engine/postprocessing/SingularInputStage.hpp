//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_SINGULARINPUTSTAGE_HPP
#define VDGE_SINGULARINPUTSTAGE_HPP

#include <engine/postprocessing/Stage.hpp>
#include <engine/postprocessing/IStageShader.hpp>

namespace mod::postprocessing {
    class SingularInputStage : public vd::postprocessing::Stage {
    public:
        explicit SingularInputStage(std::string name,
                                    vd::postprocessing::FrameBufferGetter inputGetter,
                                    vd::Predicate precondition = vd::g_kEmptyPredicate,
                                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        void Setup(const vd::Dimension& windowDimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFrameBuffer();
    private:
        vd::postprocessing::FrameBufferGetter   m_Getter;

        vd::Predicate   m_Precondition;
        vd::Consumer    m_BeforeExecution;
        vd::Consumer    m_AfterExecution;

        vd::gl::FrameBufferPtr  m_pInput;
    };
    typedef std::shared_ptr<SingularInputStage>   SingularInputStagePtr;
}


#endif //VDGE_SINGULARINPUTSTAGE_HPP
