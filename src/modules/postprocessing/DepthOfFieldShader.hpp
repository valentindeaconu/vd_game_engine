//
// Created by Vali on 2/23/2021.
//

#ifndef VDGE_DEPTHOFFIELD_HPP
#define VDGE_DEPTHOFFIELD_HPP

#include <engine/postprocessing/SingularInputStage.hpp>
#include <engine/postprocessing/IStageShader.hpp>
#include <engine/injector/ObjectOfType.hpp>

namespace mod::postprocessing {
    class DepthOfField : public vd::postprocessing::SingularInputStage {
    public:
        DepthOfField();

        void Setup(const vd::Dimension& windowDimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFBO();
    private:
        vd::postprocessing::FrameBufferGetter   m_Getter;

        vd::gl::FrameBufferPtr  m_pInput;
    };
    typedef std::shared_ptr<DepthOfField>   DepthOfFieldPtr;

    class DepthOfFieldShader : public vd::postprocessing::IStageShader, public vd::injector::Injectable {
    public:
        DepthOfFieldShader();

        void Link() override;

        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;

    private:
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<DepthOfFieldShader>   DepthOfFieldShaderPtr;

}

#endif //VDGE_DEPTHOFFIELD_HPP
