//
// Created by Vali on 9/21/2020.
//

#include "SplatMapBuilder.hpp"

namespace mod::biomes::splatmap {

    SplatMapBuilder::SplatMapBuilder() {
        m_pShader = std::make_shared<vd::gl::wrappers::Shader>();

        m_pShader->Create();

        std::string csSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/cs/SplatMap.glsl", csSource);
        m_pShader->AddShader(csSource, vd::gl::wrappers::Shader::eComputeShader);

        m_pShader->Compile();
    }

    void SplatMapBuilder::Create(const vd::gl::Texture2DPtr& heightMap,
                                 const vd::gl::Texture2DPtr& normalMap,
                                 int size,
                                 const BiomePtrVec& biomes,
                                 vd::gl::Texture2DPtr& outSplatMap,
                                 SplatMapBuilder::data_t& outData) {
        /// Create texture
        outSplatMap = vd::service::TextureService::UncachedCreateStorage(
                vd::Dimension(size, size),
                vd::gl::TextureFormat::eRGBA32F,
                int(std::log(size) / std::log(2))
        );

        outSplatMap->Bind();
        outSplatMap->LinearFilter();
        outSplatMap->Unbind();

        /// Setup Shader
        m_pShader->Bind();

        heightMap->BindToUnit(0);
        m_pShader->PushUniform("heightMap", 0);
        normalMap->BindToUnit(1);
        m_pShader->PushUniform("normalMap", 1);
        
        m_pShader->PushUniform("size", size);

        for (int i = 0; i < biomes.size(); ++i) {
            const auto prefix = "biomes[" + std::to_string(i) + "].";

            m_pShader->PushUniform(prefix + "minHeight", biomes[i]->MinimumHeight());
            m_pShader->PushUniform(prefix + "maxHeight", biomes[i]->MaximumHeight());
            m_pShader->PushUniform(prefix + "minSlope", biomes[i]->MinimumSlope());
            m_pShader->PushUniform(prefix + "maxSlope", biomes[i]->MaximumSlope());
        }

        /// Dispatch Compute Shader
        glBindImageTexture(0, outSplatMap->Id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        m_pShader->Unbind();
        normalMap->Unbind();

        /// Extract Texture Data
        outSplatMap->Bind();

        outData = std::make_shared<vd::model::Image<float, vd::model::ImageFormat::eRGBA>>(size, size);
        outData->Data().resize(size * size * 4);

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &outData->Data()[0]);

        outSplatMap->Unbind();
    }

    void SplatMapBuilder::CleanUp() {
        m_pShader->CleanUp();
    }

}