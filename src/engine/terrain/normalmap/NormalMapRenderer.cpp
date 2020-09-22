//
// Created by Vali on 9/21/2020.
//

#include "NormalMapRenderer.hpp"

namespace vd::terrain::normalmap {

    NormalMapRenderer::NormalMapRenderer(int size)
        : size(size)
    {
        shaderPtr = std::make_shared<NormalMapShader>();
        shaderPtr->addUniform("heightMap");
        shaderPtr->addUniform("size");
        shaderPtr->addUniform("strength");

        normalMap = std::make_shared<model::Texture2D>(size, size);

        normalMap->generate();
        normalMap->bind();
        normalMap->bilinearFilter();

        glTexStorage2D(GL_TEXTURE_2D, int(std::log(size) / std::log(2)), GL_RGBA32F, size, size);

        normalMap->unbind();
    }

    NormalMapRenderer::~NormalMapRenderer() = default;

    void NormalMapRenderer::render(const model::Texture2DPtr& heightMap, float strength) {
        shaderPtr->bind();

        model::activeTexture(0);
        heightMap->bind();
        shaderPtr->setUniformi("heightMap", 0);
        shaderPtr->setUniformi("size", size);
        shaderPtr->setUniformf("strength", strength);

        glBindImageTexture(0, normalMap->getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        heightMap->unbind();

        normalMap->bind();
        normalMap->bilinearFilter();
    }

    const model::Texture2DPtr& NormalMapRenderer::getNormalMap() const {
        return normalMap;
    }
}