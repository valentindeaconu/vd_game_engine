//
// Created by Vali on 11/18/2020.
//

#include "SkyShader.hpp"

namespace mod::sky {
    SkyShader::SkyShader() : vd::component::IEntity3DShader() {
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
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
    }

    void SkyShader::AddUniforms() {
        AddUniform("view");
        AddUniform("projection");

        m_pFogManager->AddUniforms(shared_from_this());
    }

    void SkyShader::InitUniforms(vd::object::Entity3DPtr pEntity) {
        AddUniforms();

        m_pFogManager->SetUniforms(shared_from_this());
    }

    void SkyShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("view", glm::mat4(glm::mat3(m_pCamera->ViewMatrix())));
        SetUniform("projection", m_pWindow->ProjectionMatrix());
    }
}