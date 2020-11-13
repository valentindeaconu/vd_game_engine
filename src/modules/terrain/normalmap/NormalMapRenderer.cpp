//
// Created by Vali on 9/21/2020.
//

#include "NormalMapRenderer.hpp"

namespace mod::terrain::normalmap {

    NormalMapRenderer::NormalMapRenderer(int size)
        : size(size)
    {
        shaderPtr = std::make_shared<NormalMapShader>();
        shaderPtr->addUniform("heightMap");
        shaderPtr->addUniform("size");
        shaderPtr->addUniform("strength");

        // TODO: Use TextureService
        normalMap = std::make_shared<vd::gl::Texture2D>(size, size);

        normalMap->Generate();
        normalMap->Bind();
        normalMap->BilinearFilter();

        glTexStorage2D(GL_TEXTURE_2D, int(std::log(size) / std::log(2)), GL_RGBA32F, size, size);

        normalMap->Unbind();
    }

    NormalMapRenderer::~NormalMapRenderer() = default;

    void NormalMapRenderer::render(const vd::gl::Texture2DPtr& heightMap, float strength) {
        shaderPtr->bind();

        vd::gl::ActiveTexture(0);
        heightMap->Bind();
        shaderPtr->setUniformi("heightMap", 0);
        shaderPtr->setUniformi("size", size);
        shaderPtr->setUniformf("strength", strength);

        glBindImageTexture(0, normalMap->Id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        heightMap->Unbind();

        normalMap->Bind();
        normalMap->BilinearFilter();
    }

    const vd::gl::Texture2DPtr& NormalMapRenderer::getNormalMap() const {
        return normalMap;
    }
}