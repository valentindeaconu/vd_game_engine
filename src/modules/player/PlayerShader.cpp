//
// Created by Vali on 11/18/2020.
//

#include "PlayerShader.hpp"

namespace mod::player {

    void PlayerShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void PlayerShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();

        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");
        AddUniform("normalMatrix");

        AddUniform("diffuseMap");
        AddUniform("specularMap");

        AddUniform("transparency");

        m_pFogManager->AddUniforms(shared_from_this());

        m_pLightManager->AddUniforms(shared_from_this());

        AddUniform("clipPlane");
    }

    void PlayerShader::InitUniforms(vd::object::Entity3DPtr pEntity) {
        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }

    void PlayerShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        glm::mat4 model = pEntity->WorldTransform().Get();
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        SetUniform("model", model);
        SetUniform("normalMatrix", normalMatrix);

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        const vd::model::Mesh3DPtr& pMesh = pEntity->Meshes(levelOfDetail)[meshIndex];

        SetUniform("transparency", 0);

        if (!pMesh->Materials().empty()) {
            vd::model::Material& meshMaterial = pMesh->Materials().front();

            if (meshMaterial.DiffuseMap() != nullptr) {
                meshMaterial.DiffuseMap()->BindToUnit(0);
                SetUniform("diffuseMap", 0);
            }

            if (meshMaterial.SpecularMap() != nullptr) {
                meshMaterial.SpecularMap()->BindToUnit(1);
                SetUniform("specularMap", 1);
            }
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }
}