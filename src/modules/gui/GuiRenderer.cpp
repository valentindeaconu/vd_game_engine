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
                m_pGuiEntity->Buffers()[0]->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);
                break;
            case eText: {
                vd::model::Mesh2DPtr& mesh = m_pGuiEntity->Meshes()[0];
                vd::model::Mesh2D::VertexVec& vertices = mesh->Vertices();
                vd::gl::BufferPtr& buffer = m_pGuiEntity->Buffers()[0];
                for (int i = 0; i < vertices.size(); i += 6) {
                    std::vector<glm::vec4> glyph(6);
                    for (int j = 0; j < 6; ++j) {
                        glyph[j] = glm::vec4(vertices[i + j].Position, vertices[i + j].TexCoords);
                    }

                    m_pShader->UpdateUniforms(m_pGuiEntity, 0, i / 6);
                    buffer->UpdateBufferData(vd::gl::eArrayBuffer, 96, &glyph[0]); // 96 = 6 vertices * 4 floats each * 4 bytes per float
                    buffer->DrawArrays(vd::gl::eTriangles, 6);
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
