//
// Created by Vali on 9/21/2020.
//

#include "SplatMapShader.hpp"

namespace mod::terrain::splatmap {
    SplatMapShader::SplatMapShader()
            : vd::shader::Shader()
    {
        loadAndAddShader("./resources/shaders/terrain/cs/SplatMap.glsl", vd::shader::eComputeShader);

        compileShader();
    }

    SplatMapShader::~SplatMapShader() = default;

    void SplatMapShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) { }
}