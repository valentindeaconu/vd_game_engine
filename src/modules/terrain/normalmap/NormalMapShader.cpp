//
// Created by Vali on 9/21/2020.
//

#include "NormalMapShader.hpp"

namespace mod::terrain::normalmap {
    NormalMapShader::NormalMapShader()
        : vd::shader::Shader()
    {
        loadAndAddShader("./resources/shaders/terrain/cs/NormalMap.glsl", vd::shader::eComputeShader);
        compileShader();
    }

    NormalMapShader::~NormalMapShader() = default;

    void NormalMapShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {

    }
}