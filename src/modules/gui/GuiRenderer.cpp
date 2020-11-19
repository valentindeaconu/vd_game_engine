//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer(GuiQuadPtr guiQuadPtr,
                             vd::gl::ShaderPtr shaderPtr,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pGuiQuad(std::move(guiQuadPtr))
    {
    }

    GuiRenderer::~GuiRenderer() = default;

    void GuiRenderer::Init() {
        m_pGuiQuad->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pGuiQuad);
    }

    void GuiRenderer::Update() {
        // TODO: Try this instead of updating uniforms each frame
        // m_pShader->Bind();
        // m_pShader->UpdateUniforms(m_pGuiQuad);
        // m_pShader->Unbind();
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

        m_pShader->Bind();

        m_pShader->UpdateUniforms(m_pGuiQuad);

        m_pGuiQuad->Buffers()[0]->Render();

        Finish();
    }

    void GuiRenderer::CleanUp() {
        m_pGuiQuad->CleanUp();
    }

    bool GuiRenderer::IsReady() {
        return IRenderer::IsReady();
    }

}
