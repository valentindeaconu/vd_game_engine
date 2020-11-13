//
// Created by Vali on 8/26/2020.
//

#include "ShadowShader.hpp"

namespace mod::shadow {
    ShadowShader::ShadowShader() : vd::shader::Shader() {
        loadAndAddShader("./resources/shaders/shadow/shadow_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/shadow/shadow_FS.glsl", vd::shader::eFragmentShader);
        compileShader();

        addUniform("model");
        addUniform("view");
        addUniform("projection");

        addUniform("diffuseMap");
    }

    ShadowShader::~ShadowShader() = default;

    void ShadowShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("model", entityPtr->WorldTransform().Get());

        auto shadowManagerPtr = vd::ObjectOfType<ShadowManager>::Find();
        setUniform("view", shadowManagerPtr->ViewMatrix());
        setUniform("projection", shadowManagerPtr->ProjectionMatrix());

        vd::model::MeshPtr& meshPtr = entityPtr->Meshes()[meshIndex];

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.diffuseMap->Bind();
                setUniformi("diffuseMap", 0);
            }
        }
    }
}