//
// Created by Vali on 2/23/2021.
//

#include "DepthOfFieldShader.hpp"

namespace mod::postprocessing {

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
        AddUniform("blurMap");
        AddUniform("near");
        AddUniform("far");
    }

    void DepthOfFieldShader::InitUniforms(vd::component::IRenderingEffectPtr pEffect) {
        AddUniforms();
    }

    void DepthOfFieldShader::UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) {
        auto pDepthOfField = std::dynamic_pointer_cast<vd::component::ConcreteEffect>(pEffect);

        SetUniform("near", m_pWindow->NearPlane());
        SetUniform("far", m_pWindow->FarPlane());

        vd::gl::ActiveTexture(0);
        pDepthOfField->InputFrameBuffer(0)->DepthTexture()->Bind();
        SetUniform("depthMap", 0);

        vd::gl::ActiveTexture(1);
        pDepthOfField->InputFrameBuffer(0)->ColorTexture()->Bind();
        SetUniform("colorMap", 1);

        vd::gl::ActiveTexture(2);
        pDepthOfField->InputFrameBuffer(1)->ColorTexture()->Bind();
        SetUniform("blurMap", 2);
    }
}