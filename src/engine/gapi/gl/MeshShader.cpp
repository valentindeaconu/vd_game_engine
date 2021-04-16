//
// Created by vali on 4/15/21.
//

#include "MeshShader.hpp"

namespace vd {

    void MeshShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void MeshShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/mesh/mesh_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/mesh/mesh_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();

        AddUniform("uModel");
        AddUniform("uView");
        AddUniform("uProjection");
        AddUniform("uNormal");

        AddUniform("uMaterialMap.DiffuseMap");
        AddUniform("uMaterialMap.SpecularMap");
        AddUniform("uMaterialMap.HorizontalScale");

        AddUniform("uTransparency");
        AddUniform("uFakeLighting");

        m_pFogManager->AddUniforms(shared_from_this());

        m_pLightManager->AddUniforms(shared_from_this());

        AddUniform("uClipPlane");
    }

    void MeshShader::InitUniforms(vd::object::Entity3DPtr pEntity) {
        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }

    void MeshShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        glm::mat4 model = pEntity->WorldTransform().Get();
        glm::mat4 view = m_pCamera->ViewMatrix();
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

        SetUniform("uModel", model);
        SetUniform("uNormal", normalMatrix);

        SetUniform("uView", view);
        SetUniform("uProjection", m_pWindow->ProjectionMatrix());

        SetUniform("uTransparency", 0);
        SetUniform("uFakeLighting", 0);

        const vd::model::MeshPtr& pMesh = pEntity->Meshes(levelOfDetail)[meshIndex];
        vd::model::MaterialMeshPtr pMaterialMesh = std::dynamic_pointer_cast<vd::model::MaterialMesh>(pMesh);
        vd::model::Material& meshMaterial = pMaterialMesh->Material();

        if (meshMaterial.DiffuseMap() != nullptr) {
            meshMaterial.DiffuseMap()->BindToUnit(0);
            SetUniform("uMaterialMap.DiffuseMap", 0);
        }

        if (meshMaterial.SpecularMap() != nullptr) {
            meshMaterial.SpecularMap()->BindToUnit(1);
            SetUniform("uMaterialMap.SpecularMap", 1);
        }
        SetUniform("uMaterialMap.HorizontalScale", 1.0f);

        SetUniform("uClipPlane", m_pContext->ClipPlane());

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }
}