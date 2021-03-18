//
// Created by Vali on 2/23/2021.
//

#include "DepthOfFieldShader.hpp"

namespace mod::postprocessing {

    DepthOfFieldShader::DepthOfFieldShader() {
        Create();
        
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

        pDepthOfField->InputFrameBuffer(0)->DepthTexture()->BindToUnit(0);
        SetUniform("depthMap", 0);

        pDepthOfField->InputFrameBuffer(0)->ColorTexture()->BindToUnit(1);
        SetUniform("colorMap", 1);

        pDepthOfField->InputFrameBuffer(1)->ColorTexture()->BindToUnit(2);
        SetUniform("blurMap", 2);
    }
}