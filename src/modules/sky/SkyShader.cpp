//
// Created by Vali on 11/18/2020.
//

#include "SkyShader.hpp"

namespace mod::sky {
    SkyShader::SkyShader()
        : vd::gl::IEntityShader()
    {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/sky_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/sky_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    SkyShader::~SkyShader() = default;

    void SkyShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pProperties = vd::ObjectOfType<vd::property::GlobalProperties>::Find();
    }

    void SkyShader::AddUniforms() {
        AddUniform("view");
        AddUniform("projection");

        AddUniform("fogDensity");
        AddUniform("fogColor");
    }

    void SkyShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();

        SetUniform("fogDensity", m_pProperties->Get<float>("Fog.SkyDensity"));
        SetUniform("fogColor", m_pProperties->Get<glm::vec3>("Fog.Color"));
    }

    void SkyShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("view", glm::mat4(glm::mat3(m_pCamera->ViewMatrix())));
        SetUniform("projection", m_pWindow->ProjectionMatrix());
    }
}