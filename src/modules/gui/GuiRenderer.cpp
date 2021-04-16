//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer(GuiQuadPtr guiQuadPtr, vd::component::IEntity2DShaderPtr shaderPtr)
        : IRenderer("GuiRenderer")
        , m_pShader(std::move(shaderPtr))
        , m_pGuiEntity(std::move(guiQuadPtr))
        , m_Type(eQuad)
    {
    }

    GuiRenderer::GuiRenderer(GuiTextPtr guiTextPtr, vd::component::IEntity2DShaderPtr shaderPtr)
            : IRenderer("GuiRenderer")
            , m_pShader(std::move(shaderPtr))
            , m_pGuiEntity(std::move(guiTextPtr))
            , m_Type(eText)
    {
    }

    void GuiRenderer::OnInit() {
        m_pGuiEntity->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pGuiEntity);
        m_pShader->Unbind();
    }

    void GuiRenderer::OnUpdate() {
        m_pGuiEntity->Update();
    }

    void GuiRenderer::OnRender(const GuiRenderer::params_t& params) {
        m_pShader->Bind();

        switch (m_Type) {
            case eQuad:
                m_pShader->UpdateUniforms(m_pGuiEntity, 0, 0);
                m_pGuiEntity->Meshes()[0]->Draw();
                break;
            case eText: {
                auto pGuiText = std::dynamic_pointer_cast<GuiText>(m_pGuiEntity);

                vd::model::MeshPtr& mesh = m_pGuiEntity->Meshes()[0];

                for (size_t i = 0; i < pGuiText->LetterMeshes().size(); ++i) {
                    auto& letterMesh = pGuiText->LetterMeshes()[i];
                    auto vertices = std::vector<vd::model::Vertex>(letterMesh.begin(), letterMesh.end());

                    m_pShader->UpdateUniforms(m_pGuiEntity, 0, i);

                    mesh->DynamicSetVertexData(vertices, false);
                    mesh->Draw();
                }
                break;
            }
        }

        m_pShader->Unbind();
    }

    void GuiRenderer::OnCleanUp() {
        m_pGuiEntity->CleanUp();
        m_pShader->CleanUp();
    }

    bool GuiRenderer::Precondition(const params_t& params) {
        if (m_pShader == nullptr || m_pGuiEntity == nullptr) {
            return false;
        }

        return (params.at("RenderingPass") == "GUI");
    }

    void GuiRenderer::Prepare() {
        if (m_Type == eQuad) {
            vd::gl::Context::CounterClockwiseFacing();
        } else {
            vd::gl::Context::ClockwiseFacing();
            vd::gl::Context::AlphaBlending();
            vd::gl::Context::NoDepthTesting();
        }
    }

    void GuiRenderer::Finish() {
        vd::gl::Context::Reset();
    }

}
