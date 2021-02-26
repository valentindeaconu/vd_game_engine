//
// Created by Vali on 2/24/2021.
//

#include "VerticalBlur.hpp"

namespace mod::postprocessing {


    VerticalBlur::VerticalBlur(vd::postprocessing::FrameBufferGetter getter)
        : vd::postprocessing::Stage("VerticalGaussianBlur")
        , m_Getter(std::move(getter))
    {
    }

    void VerticalBlur::Setup(const vd::Dimension& windowDimension) {
        m_pInput = m_Getter();

        FrameBuffer() = std::make_shared<vd::gl::FrameBuffer>(windowDimension.width, windowDimension.height);
        FrameBuffer()->Bind();
        FrameBuffer()->PushAttachment(vd::gl::FrameBuffer::eColorTexture, [](vd::gl::Texture2DPtr& pTex) {
            pTex->Bind();
            pTex->BilinearFilter();
            pTex->Unbind();
        });
        if (FrameBuffer()->Status() != vd::gl::FrameBuffer::eComplete) {
            throw vd::RuntimeError("framebuffer is incomplete or has errors");
        }
        FrameBuffer()->Unbind();
    }

    bool VerticalBlur::Precondition() {
        return true;
    }

    void VerticalBlur::Prepare() { }

    void VerticalBlur::Finish() { }

    vd::gl::FrameBufferPtr& VerticalBlur::InputFBO() {
        return m_pInput;
    }

    VerticalBlurShader::VerticalBlurShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/verticalBlur_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void VerticalBlurShader::AddUniforms() {

    }

    void VerticalBlurShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void VerticalBlurShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {

    }
}