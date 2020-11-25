//
// Created by Vali on 9/21/2020.
//

#include "NormalMapBuilder.hpp"

namespace mod::terrain::normalmap {

    NormalMapBuilder::NormalMapBuilder() {
        m_pShader = std::make_shared<vd::gl::Shader>();

        std::string csSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/cs/NormalMap.glsl", csSource);
        m_pShader->AddShader(csSource, vd::gl::Shader::eComputeShader);

        m_pShader->Compile();
    }

    NormalMapBuilder::~NormalMapBuilder() = default;

    void NormalMapBuilder::Create(const vd::gl::Texture2DPtr& heightMap,
                                  int size,
                                  float strength,
                                  vd::gl::Texture2DPtr& outNormalMap) {
        /// Create texture
        // TODO: Use TextureService
        outNormalMap = std::make_shared<vd::gl::Texture2D>(size, size);
        outNormalMap->Generate();

        outNormalMap->Bind();
        outNormalMap->BilinearFilter();
        glTexStorage2D(GL_TEXTURE_2D, int(std::log(size) / std::log(2)), GL_RGBA32F, size, size);
        outNormalMap->Unbind();

        /// Setup Shader
        m_pShader->Bind();

        vd::gl::ActiveTexture(0);
        heightMap->Bind();
        m_pShader->PushUniform("heightMap", 0);
        m_pShader->PushUniform("size", size);
        m_pShader->PushUniform("strength", strength);

        /// Dispatch Compute Shader
        glBindImageTexture(0, outNormalMap->Id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        heightMap->Unbind();
    }

}