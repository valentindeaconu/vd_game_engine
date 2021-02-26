//
// Created by Vali on 2/23/2021.
//

#include "DepthOfField.hpp"

namespace mod::postprocessing {

    DepthOfField::DepthOfField()
        : vd::postprocessing::SingularInputStage("DepthOfField",
                                                 [ctx = vd::ObjectOfType<vd::kernel::ContextPtr>]())
        , m_Getter(std::move(getter))
    {
    }

    void DepthOfField::Setup(const vd::Dimension& windowDimension) {
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

    bool DepthOfField::Precondition() {
        return true;
    }

    void DepthOfField::Prepare() { }

    void DepthOfField::Finish() { }

    vd::gl::FrameBufferPtr& DepthOfField::InputFBO() {
        return m_pInput;
    }

    DepthOfFieldShader::DepthOfFieldShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/dof_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void DepthOfFieldShader::Link() {
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void DepthOfFieldShader::AddUniforms() {
        AddUniform("colorMap");
        AddUniform("depthMap");
        AddUniform("near");
        AddUniform("far");
    }

    void DepthOfFieldShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void DepthOfFieldShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {
        DepthOfFieldPtr pDepthOfField = std::dynamic_pointer_cast<DepthOfField>(pStage);

        SetUniform("near", m_pWindow->NearPlane());
        SetUniform("far", m_pWindow->FarPlane());

        vd::gl::ActiveTexture(0);
        pDepthOfField->InputFBO()->ColorTexture()->Bind();
        SetUniform("colorMap", 0);

        vd::gl::ActiveTexture(1);
        pDepthOfField->InputFBO()->DepthTexture()->Bind();
        SetUniform("depthMap", 1);
    }
}