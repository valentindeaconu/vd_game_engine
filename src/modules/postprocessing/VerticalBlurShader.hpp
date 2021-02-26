//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_VERTICALBLUR_HPP
#define VDGE_VERTICALBLUR_HPP

#include <engine/postprocessing/Stage.hpp>
#include <engine/postprocessing/IStageShader.hpp>

namespace mod::postprocessing {
    class VerticalBlur : public vd::postprocessing::Stage {
    public:
        explicit VerticalBlur(vd::postprocessing::FrameBufferGetter getter);

        void Setup(const vd::Dimension& windowDimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFBO();
    private:
        vd::postprocessing::FrameBufferGetter   m_Getter;

        vd::gl::FrameBufferPtr  m_pInput;
    };
    typedef std::shared_ptr<VerticalBlur>   VerticalBlurPtr;

    class VerticalBlurShader : public vd::postprocessing::IStageShader {
    public:
        VerticalBlurShader();

        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<VerticalBlurShader>   VerticalBlurShaderPtr;
}


#endif //VDGE_VERTICALBLUR_HPP
