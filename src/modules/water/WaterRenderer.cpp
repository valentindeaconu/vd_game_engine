//
// Created by Vali on 9/11/2020.
//

#include "WaterRenderer.hpp"

namespace mod::water {

    WaterRenderer::WaterRenderer()
        : vd::component::Renderer()
        , waterPtr(nullptr)
    {
    }

    WaterRenderer::~WaterRenderer() = default;

    void WaterRenderer::init() {
        waterPtr->init();
    }

    void WaterRenderer::update() {
        waterPtr->update();
    }

    void WaterRenderer::render(const vd::kernel::RenderingPass& renderingPass) {
        if (isReady() && renderingPass == vd::kernel::RenderingPass::eMain) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            shaderPtr->bind();
            shaderPtr->updateUniforms(waterPtr, 0);
            waterPtr->getMeshBuffers()[0]->render();

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void WaterRenderer::cleanUp() {
        waterPtr->cleanUp();
    }

    WaterPtr &WaterRenderer::getWater() {
        return waterPtr;
    }

    const WaterPtr &WaterRenderer::getWater() const {
        return waterPtr;
    }

    void WaterRenderer::setWater(const WaterPtr &waterPtr) {
        this->waterPtr = waterPtr;
    }

    bool WaterRenderer::isReady() {
        return Renderer::isReady() && waterPtr != nullptr;
    }
}