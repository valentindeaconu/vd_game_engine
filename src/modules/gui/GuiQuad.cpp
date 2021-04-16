//
// Created by Vali on 8/29/2020.
//

#include "GuiQuad.hpp"

namespace mod::gui {
    GuiQuad::GuiQuad(vd::gl::Texture2DPtr texture, const glm::vec2& position, const glm::vec2& scale)
       : m_Texture(std::move(texture))
    {
       this->LocalTransform().Translation() = glm::vec3(position.x, position.y, 0.0f);
       this->LocalTransform().Scale() = glm::vec3(scale.x, scale.y, 1.0f);
    }

    GuiQuad::GuiQuad(GuiQuad::TextureGetter textureGetter, const glm::vec2& position, const glm::vec2& scale)
        : m_TextureGetter(std::move(textureGetter))
    {
        this->LocalTransform().Translation() = glm::vec3(position.x, position.y, 0.0f);
        this->LocalTransform().Scale() = glm::vec3(scale.x, scale.y, 1.0f);
    }

    GuiQuad::~GuiQuad() = default;

    void GuiQuad::Setup() {
        if (m_Texture == nullptr) {
            m_Texture = m_TextureGetter();
        }

        vd::model::MeshPtr& pMesh = Meshes()
                .emplace_back(std::make_shared<vd::model::Mesh>(vd::gapi::AttributeTypeVec({
                    vd::gapi::AttributeType::FLOAT_2
                })));

        pMesh->Assign(vd::gapi::DataFragmentation::eAsTriangles,
        {
            vd::model::Vertex(std::vector<float>{ -1.0f, 1.0f }),
            vd::model::Vertex(std::vector<float>{ -1.0f, -1.0f }),
            vd::model::Vertex(std::vector<float>{ 1.0f, 1.0f }),
            vd::model::Vertex(std::vector<float>{ 1.0f, -1.0f })
        },
        {0, 1, 2, 2, 1, 3});

        pMesh->Material().DiffuseMap() = m_Texture;
    }

    void GuiQuad::Update() {

    }

}