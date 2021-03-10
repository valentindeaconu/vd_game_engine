//
// Created by Vali on 9/21/2020.
//

#include "SplatMapBuilder.hpp"

namespace mod::terrain::splatmap {

    SplatMapBuilder::SplatMapBuilder() {
        m_pShader = std::make_shared<vd::gl::Shader>();

        m_pShader->Create();

        std::string csSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/cs/SplatMap.glsl", csSource);
        m_pShader->AddShader(csSource, vd::gl::Shader::eComputeShader);

        m_pShader->Compile();
    }

    SplatMapBuilder::~SplatMapBuilder() = default;

    void SplatMapBuilder::Create(const vd::gl::Texture2DPtr& heightMap,
                                 int size,
                                 float scaleY,
                                 const BiomePtrVec& biomes,
                                 vd::gl::Texture2DPtr& outSplatMap,
                                 SplatMapBuilder::data_t& outData) {
        /// Create texture
        outSplatMap = vd::service::TextureService::UncachedCreateStorage(
                vd::Dimension(size, size),
                vd::gl::TextureFormat::eR32UI,
                int(std::log(size) / std::log(2))
        );

        outSplatMap->Bind();
        outSplatMap->NoFilter();
        outSplatMap->Unbind();

        /// Setup Shader
        m_pShader->Bind();

        vd::gl::ActiveTexture(0);
        heightMap->Bind();
        m_pShader->PushUniform("heightMap", 0);
        m_pShader->PushUniform("size", size);
        m_pShader->PushUniform("scaleY", scaleY);

        for (int i = 0; i < biomes.size(); ++i) {
            const auto& biomePtr = biomes[i];

            const std::string prefix = "biomes[" + std::to_string(i) + "]";

            m_pShader->PushUniform(prefix + ".minHeight", biomePtr->MinimumHeight());
            m_pShader->PushUniform(prefix + ".maxHeight", biomePtr->MaximumHeight());
        }

        /// Dispatch Compute Shader
        glBindImageTexture(0, outSplatMap->Id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        heightMap->Unbind();

        /// Extract Texture Data
        outSplatMap->Bind();

        outData = std::make_shared<vd::model::Image<uint32_t, vd::model::ImageFormat::eR>>(size, size);
        outData->Data().resize(size * size);

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &outData->Data()[0]);

        outSplatMap->Unbind();
    }

    void SplatMapBuilder::CleanUp() {
        m_pShader->CleanUp();
    }

}