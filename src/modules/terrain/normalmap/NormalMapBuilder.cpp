//
// Created by Vali on 9/21/2020.
//

#include "NormalMapBuilder.hpp"

namespace mod::terrain::normalmap {

    NormalMapBuilder::NormalMapBuilder() {
        m_pShader = std::make_shared<vd::gl::Shader>();

        m_pShader->Create();

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
        outNormalMap = vd::service::TextureService::UncachedCreateStorage(
                vd::Dimension(size, size),
                vd::gl::TextureFormat::eRGBA32F,
                int(std::log(size) / std::log(2))
        );

        outNormalMap->Bind();
        outNormalMap->LinearFilter();
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

    void NormalMapBuilder::CleanUp() {
        m_pShader->CleanUp();
    }

}