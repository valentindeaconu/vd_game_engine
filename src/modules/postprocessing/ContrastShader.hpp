//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_CONTRAST_HPP
#define VDGE_CONTRAST_HPP

#include <engine/postprocessing/Stage.hpp>
#include <engine/postprocessing/IStageShader.hpp>

namespace mod::postprocessing {
    class Contrast : public vd::postprocessing::Stage {
    public:
        explicit Contrast(vd::postprocessing::FrameBufferGetter getter);

        void Setup(const vd::Dimension& windowDimension) override;

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFBO();
    private:
        vd::postprocessing::FrameBufferGetter   m_Getter;

        vd::gl::FrameBufferPtr  m_pInput;
    };
    typedef std::shared_ptr<Contrast>   ContrastPtr;

    class ContrastShader : public vd::postprocessing::IStageShader {
    public:
        ContrastShader();
        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<ContrastShader>   ContrastShaderPtr;

}


#endif //VDGE_CONTRAST_HPP
