//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer()
        : Renderer()
        , m_GuiQuadPtr(nullptr)
        , m_Initialised(false)
    {
    }

    GuiRenderer::~GuiRenderer() = default;

    void GuiRenderer::Init() {
        if (!m_Initialised) {
            m_GuiQuadPtr->init();
            m_Initialised = false;
        }
    }

    void GuiRenderer::Update() {

    }

    void GuiRenderer::Render(const vd::kernel::RenderingPass& renderingPass) {
        Init();

        if (!IsReady() || renderingPass != vd::kernel::RenderingPass::eMain)
            return;

        if (m_ConfigPtr != nullptr)
            m_ConfigPtr->enable();

        m_ShaderPtr->bind();
        m_ShaderPtr->updateUniforms(m_GuiQuadPtr, 0);
        m_GuiQuadPtr->getBuffers()[0]->render();

        if (m_ConfigPtr != nullptr)
            m_ConfigPtr->disable();
    }

    void GuiRenderer::CleanUp() {
        m_GuiQuadPtr->cleanUp();
    }

    GuiQuadPtr &GuiRenderer::GetGuiQuad() {
        return m_GuiQuadPtr;
    }

    const GuiQuadPtr &GuiRenderer::GetGuiQuad() const {
        return m_GuiQuadPtr;
    }

    void GuiRenderer::SetGuiQuad(const GuiQuadPtr& guiQuadPtr) {
        this->m_GuiQuadPtr = guiQuadPtr;
    }

    bool GuiRenderer::IsReady() {
        return Renderer::IsReady();
    }

}
