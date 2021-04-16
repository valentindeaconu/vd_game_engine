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
        vd::model::MeshPtr& pMesh = Meshes()
                .emplace_back(
                        std::move(
                                std::make_shared<vd::model::Mesh>(
                                        vd::gapi::AttributeTypeVec ({
                                                  vd::gapi::AttributeType::FLOAT_4,
                                          }))));

        // Mesh is reused for each letter due to no atlas for font (each letter has to be bound in order to use its texture)
        pMesh->MakeDynamic(vd::gapi::DataFragmentation::eAsTriangles,
                           24, // 4 floats/vertex * 6 vertex/letter
                           0);
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

    GuiText::LetterMeshVec& GuiText::LetterMeshes() {
        return m_LetterMeshes;
    }

    void GuiText::Color(const glm::vec3& color) {
        vd::model::MeshPtr pMesh = Meshes()[0];
        pMesh->Material().Color() = glm::vec4(color, 1.0f);
        m_Color = color;
    }

    void GuiText::Rebuild() {
        float x = m_Position.x;
        float y = m_Position.y;

        m_LetterMeshes.clear();

        for (char& c : m_Text) {
            vd::model::Character ch = m_pFont->Characters()[c];

            float xpos = x + float(ch.Bearing.x) * m_Scale;
            float ypos = y + float(ch.Size.y - ch.Bearing.y) * m_Scale;

            float w = float(ch.Size.x) * m_Scale;
            float h = float(ch.Size.y) * m_Scale;

            auto& letterMesh = m_LetterMeshes.emplace_back();

            letterMesh[0] = vd::model::Vertex(std::vector<float>({ xpos, ypos + h, 0.0, 1.0 }));
            letterMesh[1] = vd::model::Vertex(std::vector<float>({ xpos + w, ypos, 1.0, 0.0 }));
            letterMesh[2] = vd::model::Vertex(std::vector<float>({ xpos, ypos, 0.0, 0.0 }));

            letterMesh[3] = vd::model::Vertex(std::vector<float>({ xpos, ypos + h, 0.0, 1.0 }));
            letterMesh[4] = vd::model::Vertex(std::vector<float>({ xpos + w, ypos + h, 1.0, 1.0 }));
            letterMesh[5] = vd::model::Vertex(std::vector<float>({ xpos + w, ypos, 1.0, 0.0 }));

            // Now advance cursors for next glyph
            x += float(ch.Advance >> 6) * m_Scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
        }

        auto& pMesh = Meshes()[0];
        pMesh->Material().Color() = glm::vec4(m_Color, 1.0f);
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