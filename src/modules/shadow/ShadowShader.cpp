//
// Created by Vali on 8/26/2020.
//

#include "ShadowShader.hpp"

namespace mod::shadow {
    ShadowShader::ShadowShader()
            : vd::gl::IEntityShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/shadow/shadow_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/shadow/shadow_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();;
    }

    ShadowShader::~ShadowShader() = default;

    void ShadowShader::Link() {
        m_pShadowManager = vd::ObjectOfType<ShadowManager>::Find();
    }

    void ShadowShader::AddUniforms() {
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");
    }

    void ShadowShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();
    }

    void ShadowShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());

        SetUniform("view", m_pShadowManager->ViewMatrix());
        SetUniform("projection", m_pShadowManager->ProjectionMatrix());

        vd::model::MeshPtr& meshPtr = pEntity->Meshes()[meshIndex];

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.diffuseMap->Bind();
                SetUniform("diffuseMap", 0);
            }
        }
    }

}