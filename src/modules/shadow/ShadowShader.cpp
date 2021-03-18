//
// Created by Vali on 8/26/2020.
//

#include "ShadowShader.hpp"

namespace mod::shadow {

    void ShadowShader::Link() {
        m_pShadowManager = vd::ObjectOfType<ShadowManager>::Find();
    }

    void ShadowShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/shadow/shadow_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/shadow/shadow_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
        
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");
    }

    void ShadowShader::InitUniforms(vd::object::Entity3DPtr pEntity) {

    }

    void ShadowShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());

        SetUniform("view", m_pShadowManager->ViewMatrix());
        SetUniform("projection", m_pShadowManager->ProjectionMatrix());

        const vd::model::Mesh3DPtr& pMesh = pEntity->Meshes(levelOfDetail)[meshIndex];

        if (!pMesh->Materials().empty()) {
            vd::model::Material& meshMaterial = pMesh->Materials().front();

            if (meshMaterial.DiffuseMap() != nullptr) {
                meshMaterial.DiffuseMap()->BindToUnit(0);
                SetUniform("diffuseMap", 0);
            }
        }
    }

}