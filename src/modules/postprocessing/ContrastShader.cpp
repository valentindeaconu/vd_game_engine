//
// Created by Vali on 2/19/2021.
//

#include "Contrast.hpp"

namespace mod::postprocessing {
    Contrast::Contrast(vd::postprocessing::FrameBufferGetter getter)
        : vd::postprocessing::Stage("Contrast")
        , m_Getter(std::move(getter))
    {
    }

    void Contrast::Setup(const vd::Dimension& windowDimension) {
        m_pInput = m_Getter();

        FrameBuffer() = std::make_shared<vd::gl::FrameBuffer>(windowDimension.width, windowDimension.height);
        FrameBuffer()->Bind();
        FrameBuffer()->PushAttachment(vd::gl::FrameBuffer::eColorTexture);
        if (FrameBuffer()->Status() != vd::gl::FrameBuffer::eComplete) {
            throw vd::RuntimeError("framebuffer is incomplete or has errors");
        }
        FrameBuffer()->Unbind();
    }

    bool Contrast::Precondition() {
        return true;
    }

    void Contrast::Prepare() {

    }

    void Contrast::Finish() {

    }

    vd::gl::FrameBufferPtr& Contrast::InputFBO() {
        return m_pInput;
    }

    ContrastShader::ContrastShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/contrast_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void ContrastShader::AddUniforms() {
        AddUniform("colorMap");
    }

    void ContrastShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void ContrastShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {
        ContrastPtr pContrast = std::dynamic_pointer_cast<Contrast>(pStage);

        vd::gl::ActiveTexture(0);
        pContrast->InputFBO()->ColorTexture()->Bind();
        SetUniform("colorMap", 0);
    }


}

