#include "SkyRenderer.hpp"

namespace mod::sky {
    SkyRenderer::SkyRenderer()
        : Renderer()
        , skyPtr(nullptr)
    {
    }

    SkyRenderer::~SkyRenderer() = default;

    void SkyRenderer::init() {
        skyPtr->init();
    }

    void SkyRenderer::update() {
        skyPtr->update();
    }

    void SkyRenderer::render(const vd::kernel::RenderingPass& renderingPass) {
        if (isReady()) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->getShadowShader() : shaderPtr;

            _shaderPtr->bind();
            vd::buffer::BufferPtrVec& buffers = skyPtr->getBuffers();
            for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
                _shaderPtr->updateUniforms(skyPtr, meshIndex);
                buffers[meshIndex]->render();
            }

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void SkyRenderer::cleanUp() {
        skyPtr->cleanUp();
    }

    SkyPtr& SkyRenderer::getSky() {
        return skyPtr;
    }

    const SkyPtr& SkyRenderer::getSky() const {
        return skyPtr;
    }

    void SkyRenderer::setSky(const SkyPtr& skyPtr) {
        this->skyPtr = skyPtr;
    }

    bool SkyRenderer::isReady()
    {
        return Renderer::isReady() && skyPtr != nullptr;
    }
}