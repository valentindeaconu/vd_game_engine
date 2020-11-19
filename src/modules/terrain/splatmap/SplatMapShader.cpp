//
// Created by Vali on 9/21/2020.
//

#include "SplatMapShader.hpp"

namespace mod::terrain::splatmap {
    SplatMapShader::SplatMapShader()
            : vd::gl::Shader()
    {
        std::string csSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/cs/SplatMap.glsl", csSource);
        AddShader(csSource, vd::gl::Shader::eComputeShader);

        Compile();
    }

    SplatMapShader::~SplatMapShader() = default;

    void SplatMapShader::AddUniforms() {

    }

    void SplatMapShader::InitUniforms(vd::object::EntityPtr pEntity) {

    }

    void SplatMapShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {

    }
}