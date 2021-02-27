//
// Created by Vali on 8/29/2020.
//

#include "GuiRenderer.hpp"

namespace mod::gui {

    GuiRenderer::GuiRenderer(GuiQuadPtr guiQuadPtr,
                             vd::component::IEntityShaderPtr shaderPtr,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pGuiEntity(std::move(guiQuadPtr))
        , m_Type(eQuad)
    {
    }

    GuiRenderer::GuiRenderer(GuiTextPtr guiTextPtr,
                             vd::component::IEntityShaderPtr shaderPtr,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
            : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
            , m_pGuiEntity(std::move(guiTextPtr))
            , m_Type(eText)
    {
    }

    void GuiRenderer::Init() {
        m_pGuiEntity->Init();

        m_pShader->Bind();

        m_pShader->InitUniforms(m_pGuiEntity);
    }

    void GuiRenderer::Update() {
        m_pGuiEntity->Update();
    }

    void GuiRenderer::Render(const GuiRenderer::params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("GuiRenderer was not ready to render");
            return;
        }

        if (params.at("RenderingPass") != "GUI") {
            return;
        }

        Prepare();

        m_pShader->Bind();

        switch (m_Type) {
            case eQuad:
                m_pShader->UpdateUniforms(m_pGuiEntity, 0);
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

                    m_pShader->UpdateUniforms(m_pGuiEntity, i / 6);
                    buffer->UpdateBufferData(vd::gl::buffer::eArrayBuffer, 96, &glyph[0]); // 96 = 6 vertices * 4 floats each * 4 bytes per float
                    buffer->DrawArrays(vd::gl::eTriangles, 6);
                }
                break;
            }
        }

        Finish();
    }

    void GuiRenderer::CleanUp() {
        m_pGuiEntity->CleanUp();
    }

    bool GuiRenderer::IsReady() {
        return IRenderer::IsReady();
    }

}
