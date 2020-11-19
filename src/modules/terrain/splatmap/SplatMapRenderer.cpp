//
// Created by Vali on 9/21/2020.
//

#include "SplatMapRenderer.hpp"

namespace mod::terrain::splatmap {

    SplatMapRenderer::SplatMapRenderer(int size)
            : size(size)
            , kBiomeCount(5)
    {
        shaderPtr = std::make_shared<SplatMapShader>();
        shaderPtr->AddUniform("heightMap");
        shaderPtr->AddUniform("size");
        shaderPtr->AddUniform("scaleY");

        for (int i = 0; i < kBiomeCount; ++i) {
            shaderPtr->AddUniform("biomes[" + std::to_string(i) + "].minHeight");
            shaderPtr->AddUniform("biomes[" + std::to_string(i) + "].maxHeight");
        }

        // TODO: Use TextureService
        splatMap = std::make_shared<vd::gl::Texture2D>(size, size);

        splatMap->Generate();
        splatMap->Bind();
        splatMap->BilinearFilter();

        glTexStorage2D(GL_TEXTURE_2D, int(std::log(size) / std::log(2)), GL_R32UI, size, size);

        splatMap->Unbind();
    }

    SplatMapRenderer::~SplatMapRenderer() = default;

    void SplatMapRenderer::render(const vd::gl::Texture2DPtr& heightMap, float scaleY, const BiomePtrVec& biomes) {
        shaderPtr->Bind();

        vd::gl::ActiveTexture(0);
        heightMap->Bind();
        shaderPtr->SetUniform("heightMap", 0);
        shaderPtr->SetUniform("size", size);
        shaderPtr->SetUniform("scaleY", scaleY);

        for (int i = 0; i < kBiomeCount; ++i) {
            const auto& biomePtr = biomes[i];
            shaderPtr->SetUniform("biomes[" + std::to_string(i) + "].minHeight", biomePtr->getMinHeight());
            shaderPtr->SetUniform("biomes[" + std::to_string(i) + "].maxHeight", biomePtr->getMaxHeight());
        }

        glBindImageTexture(0, splatMap->Id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        heightMap->Unbind();

        splatMap->Bind();
        splatMap->BilinearFilter();

        splatImg = std::make_shared<vd::model::Image<uint32_t, vd::model::ImageFormat::eR>>(size, size);
        splatImg->Data().resize(size * size);

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &splatImg->Data()[0]); //&outBuffer[0]);

        /*if (std::any_of(splatImg->Data().begin(), splatImg->Data().end(), [](auto x) {
            return x == 2;
        })) {
            int a = 0;
            ++a;
        }*/

        splatMap->Unbind();
    }

    const vd::gl::Texture2DPtr& SplatMapRenderer::getSplatMap() const {
        return splatMap;
    }

    const vd::model::ImagePtr<uint32_t, vd::model::ImageFormat::eR>& SplatMapRenderer::getSplatData() const {
        return splatImg;
    }
}