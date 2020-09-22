//
// Created by Vali on 9/21/2020.
//

#include "SplatMapShader.hpp"

namespace vd::terrain::splatmap {
    SplatMapShader::SplatMapShader()
            : shader::Shader()
    {
        loadAndAddShader("./resources/shaders/terrain2/cs/SplatMap.glsl", vd::shader::eComputeShader);

        compileShader();
    }

    SplatMapShader::~SplatMapShader() = default;

    void SplatMapShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) { }
}