//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer(GuiQuadPtr guiQuadPtr,
                             vd::shader::ShaderPtr shaderPtr,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_GuiQuadPtr(std::move(guiQuadPtr))
    {
    }

    GuiRenderer::~GuiRenderer() = default;

    void GuiRenderer::Init() {
        m_GuiQuadPtr->Init();
    }

    void GuiRenderer::Update() {

    }

    void GuiRenderer::Render(const GuiRenderer::params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("GuiRenderer was not ready to render");
            return;
        }

        if (params.at("RenderingPass") != "Main") {
            return;
        }

        Prepare();

        m_ShaderPtr->bind();

        m_ShaderPtr->updateUniforms(m_GuiQuadPtr, 0);

        m_GuiQuadPtr->Buffers()[0]->Render();

        Finish();
    }

    void GuiRenderer::CleanUp() {
        m_GuiQuadPtr->CleanUp();
    }

    bool GuiRenderer::IsReady() {
        return IRenderer::IsReady();
    }

}
