//
// Created by Vali on 4/6/2021.
//

#include "LensFlareShader.hpp"

namespace mod::sky {

    void FlareShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }
    
    void FlareShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/flare_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::wrappers::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/flare_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::wrappers::Shader::eFragmentShader);

        Compile();

        AddUniform("transform");
        AddUniform("brightness");

        AddUniform("diffuseMap");
    }

    void FlareShader::UpdateUniforms(const vd::gl::Texture2DPtr& texture, const glm::vec4& transform, float brightness) {
        SetUniform("transform", transform); 
        SetUniform("brightness", brightness);

        texture->BindToUnit(0);
        SetUniform("diffuseMap", 0);        
    }
    
}