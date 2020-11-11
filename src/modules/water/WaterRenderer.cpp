//
// Created by Vali on 9/11/2020.
//

#include "WaterRenderer.hpp"

namespace mod::water {

    WaterRenderer::WaterRenderer()
        : vd::component::Renderer()
        , m_WaterPtr(nullptr)
    {
    }

    WaterRenderer::~WaterRenderer() = default;

    void WaterRenderer::Init() {
        m_WaterPtr->init();
    }

    void WaterRenderer::Update() {
        m_WaterPtr->update();
    }

    void WaterRenderer::Render(const vd::kernel::RenderingPass& renderingPass) {
        if (IsReady() && renderingPass == vd::kernel::RenderingPass::eMain) {
            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->enable();
            }

            m_ShaderPtr->bind();
            m_ShaderPtr->updateUniforms(m_WaterPtr, 0);
            m_WaterPtr->getBuffers()[0]->render();

            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->disable();
            }
        }
    }

    void WaterRenderer::CleanUp() {
        m_WaterPtr->cleanUp();
    }

    WaterPtr &WaterRenderer::GetWater() {
        return m_WaterPtr;
    }

    const WaterPtr &WaterRenderer::GetWater() const {
        return m_WaterPtr;
    }

    void WaterRenderer::SetWater(const WaterPtr &waterPtr) {
        this->m_WaterPtr = waterPtr;
    }

    bool WaterRenderer::IsReady() {
        return Renderer::IsReady() && m_WaterPtr != nullptr;
    }
}