#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer()
        : Renderer()
        , playerPtr(nullptr)
    {
    }

    PlayerRenderer::~PlayerRenderer() = default;

    void PlayerRenderer::init() {
        playerPtr->init();
    }

    void PlayerRenderer::update() {
        playerPtr->update();
    }

    void PlayerRenderer::render(const vd::kernel::RenderingPass& renderingPass) {
        if (isReady() && renderingPass != vd::kernel::eReflection && renderingPass != vd::kernel::eRefraction) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->getShadowShader() : shaderPtr;

            _shaderPtr->bind();
            vd::buffer::BufferPtrVec& buffers = playerPtr->getBuffers();
            for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
                _shaderPtr->updateUniforms(playerPtr, meshIndex);
                buffers[meshIndex]->render();
            }

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void PlayerRenderer::cleanUp() {
        playerPtr->cleanUp();
    }

    PlayerPtr& PlayerRenderer::getPlayer() {
        return playerPtr;
    }

    const PlayerPtr& PlayerRenderer::getPlayer() const {
        return playerPtr;
    }

    void PlayerRenderer::setPlayer(const PlayerPtr& playerPtr) {
        this->playerPtr = playerPtr;
    }

    bool PlayerRenderer::isReady() {
        return Renderer::isReady() && playerPtr != nullptr;
    }
}