//
// Created by Vali on 11/18/2020.
//

#include "PlayerShader.hpp"

namespace mod::player {
    PlayerShader::PlayerShader() : vd::component::IEntityShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    PlayerShader::~PlayerShader() = default;

    void PlayerShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
        m_pContext = vd::ObjectOfType<vd::kernel::Context>::Find();
    }

    void PlayerShader::AddUniforms() {
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");
        AddUniform("specularMap");

        AddUniform("transparency");

        m_pFogManager->AddUniforms(shared_from_this());

        m_pLightManager->AddUniforms(shared_from_this());

        AddUniform("clipPlane");
    }

    void PlayerShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }

    void PlayerShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());
        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        vd::model::MeshPtr& pMesh = pEntity->Meshes()[meshIndex];

        SetUniform("transparency", 0);

        if (!pMesh->Materials().empty()) {
            vd::model::Material& meshMaterial = pMesh->Materials().front();

            if (meshMaterial.DiffuseMap() != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.DiffuseMap()->Bind();
                SetUniform("diffuseMap", 0);
            }

            if (meshMaterial.SpecularMap() != nullptr) {
                vd::gl::ActiveTexture(1);
                meshMaterial.SpecularMap()->Bind();
                SetUniform("specularMap", 1);
            }
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());
    }
}