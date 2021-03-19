//
// Created by Vali on 2/18/2021.
//

#include "GuiText.hpp"

namespace mod::gui {

    GuiText::GuiText(std::string text,
                     vd::model::FontPtr font,
                     const glm::vec2& position,
                     float scale,
                     const glm::vec3& color)
        : m_Text(std::move(text))
        , m_pFont(std::move(font))
        , m_Position(position)
        , m_Scale(scale)
        , m_Color(color)
    {
    }

    void GuiText::Setup() {
        Buffers().clear();

        vd::gl::BufferPtr& pBuffer = Buffers().emplace_back(std::move(std::make_shared<vd::gl::Buffer>()));

        pBuffer->Create();
        pBuffer->Bind();

        pBuffer->AddBuffer(vd::gl::eArrayBuffer, sizeof(float) * 6 * 4, nullptr, vd::gl::eDynamicDraw);
        pBuffer->AttributeArray(0, 4, vd::gl::eFloat, 4 * sizeof(float), nullptr);

        pBuffer->Unbind();
    }

    void GuiText::Init() {
        Setup();

        Rebuild();
    }

    void GuiText::Update() { }

    std::string& GuiText::Text() {
        return m_Text;
    }

    vd::model::FontPtr& GuiText::Font() {
        return m_pFont;
    }

    float& GuiText::Scale() {
        return m_Scale;
    }

    glm::vec2& GuiText::Position() {
        return m_Position;
    }

    void GuiText::Color(const glm::vec3& color) {
        Meshes()[0]->Materials()[0].Color() = glm::vec4(color, 1.0f);
        m_Color = color;
    }

    void GuiText::Rebuild() {
        Meshes().clear();

        float x = m_Position.x;
        float y = m_Position.y;

        vd::model::Mesh2DPtr& pMesh = Meshes().emplace_back(std::move(std::make_shared<vd::model::Mesh2D>()));

        pMesh->Vertices().reserve(m_Text.size() * 6);

        for (auto it = m_Text.begin(); it != m_Text.end(); ++it) {
            vd::model::Character ch = m_pFont->Characters()[*it];

            float xpos = x + ch.Bearing.x * m_Scale;
            float ypos = y + float(ch.Size.y - ch.Bearing.y) * m_Scale;

            float w = ch.Size.x * m_Scale;
            float h = ch.Size.y * m_Scale;

            pMesh->Vertices().emplace_back(xpos, ypos + h, 0.0, 1.0);
            pMesh->Vertices().emplace_back(xpos + w, ypos, 1.0, 0.0);
            pMesh->Vertices().emplace_back(xpos, ypos, 0.0, 0.0);

            pMesh->Vertices().emplace_back(xpos, ypos + h, 0.0, 1.0);
            pMesh->Vertices().emplace_back(xpos + w, ypos + h, 1.0, 1.0);
            pMesh->Vertices().emplace_back(xpos + w, ypos, 1.0, 0.0);

            // Now advance cursors for next glyph
            x += (ch.Advance >> 6) * m_Scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
        }

        pMesh->Materials().emplace_back();
        pMesh->Materials().back().Color() = glm::vec4(m_Color, 1.0f);
    }

    UpdatableGuiText::UpdatableGuiText(std::string text,
                                       vd::model::FontPtr font,
                                       UpdateConsumer updateConsumer,
                                       const glm::vec2& position,
                                       float scale,
                                       const glm::vec3& color)
        : GuiText(std::move(text), std::move(font), position, scale, color)
        , m_UpdateConsumer(std::move(updateConsumer))
    {
    }

    void UpdatableGuiText::Update() {
        m_UpdateConsumer(*this);
    }

    bool UpdatableGuiText::Contains(const std::string& param) const {
        return m_Params.find(param) != m_Params.end();
    }

    std::string& UpdatableGuiText::operator[](const std::string& param) {
        return m_Params[param];
    }
}