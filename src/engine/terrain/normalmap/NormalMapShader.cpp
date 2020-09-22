//
// Created by Vali on 9/21/2020.
//

#include "NormalMapShader.hpp"

namespace vd::terrain::normalmap {
    NormalMapShader::NormalMapShader()
        : shader::Shader()
    {
        loadAndAddShader("./resources/shaders/terrain2/cs/NormalMap.glsl", vd::shader::eComputeShader);
        compileShader();
    }

    NormalMapShader::~NormalMapShader() = default;

    void NormalMapShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {

    }
}