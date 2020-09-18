//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer()
        : Renderer()
        , guiQuadPtr(nullptr)
        , initialised(false)
    {
    }

    GuiRenderer::~GuiRenderer() = default;

    void GuiRenderer::init() {
        if (!initialised) {
            guiQuadPtr->init();
            initialised = false;
        }
    }

    void GuiRenderer::update() {

    }

    void GuiRenderer::render(const vd::kernel::RenderingPass& renderingPass) {
        init();

        if (!isReady() || renderingPass != vd::kernel::RenderingPass::eMain)
            return;

        if (renderConfigPtr != nullptr)
            renderConfigPtr->enable();

        shaderPtr->bind();
        shaderPtr->updateUniforms(guiQuadPtr, 0);
        guiQuadPtr->getMeshBuffers()[0]->render();

        if (renderConfigPtr != nullptr)
            renderConfigPtr->disable();
    }

    void GuiRenderer::cleanUp() {
        guiQuadPtr->cleanUp();
    }

    GuiQuadPtr &GuiRenderer::getGuiQuad() {
        return guiQuadPtr;
    }

    const GuiQuadPtr &GuiRenderer::getGuiQuad() const {
        return guiQuadPtr;
    }

    void GuiRenderer::setGuiQuad(const GuiQuadPtr& guiQuadPtr) {
        this->guiQuadPtr = guiQuadPtr;
    }

    bool GuiRenderer::isReady() {
        return Renderer::isReady();
    }

}
