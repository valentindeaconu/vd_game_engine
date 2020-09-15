//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer()
        : Renderer()
        , guiQuadPtr(nullptr)
        , firstTime(true)
    {

    }

    GuiRenderer::~GuiRenderer() = default;

    void GuiRenderer::init() {
        guiQuadPtr->init();
    }

    void GuiRenderer::update(bool shadowUpdate) {
        if (firstTime) {
            this->init();
            firstTime = false;
        }

        if (!isReady() || shadowUpdate)
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
