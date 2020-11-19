//
// Created by Vali on 9/21/2020.
//

#include "NormalMapShader.hpp"

namespace mod::terrain::normalmap {
    NormalMapShader::NormalMapShader()
        : vd::gl::Shader()
    {
        std::string csSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/cs/NormalMap.glsl", csSource);
        AddShader(csSource, vd::gl::Shader::eComputeShader);

        Compile();
    }

    NormalMapShader::~NormalMapShader() = default;

    void NormalMapShader::AddUniforms() {

    }

    void NormalMapShader::InitUniforms(vd::object::EntityPtr pEntity) {

    }

    void NormalMapShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {

    }
}