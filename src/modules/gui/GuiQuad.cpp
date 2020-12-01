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

    void GuiQuad::Init() {
        if (m_Texture == nullptr) {
            m_Texture = m_TextureGetter();
        }

        vd::model::MeshPtrVec& meshPtrVec = Meshes();

        vd::model::MeshPtr pMesh = std::make_shared<vd::model::Mesh>();

        pMesh->Vertices() = {
            vd::model::Vertex(-1.0f, 1.0f, 0.0f),
            vd::model::Vertex(-1.0f, -1.0f, 0.0f),
            vd::model::Vertex(1.0f, 1.0f, 0.0f),
            vd::model::Vertex(1.0f, -1.0f, 0.0f)
        };

        pMesh->Indices() = {0, 1, 2, 2, 1, 3 };

        pMesh->Materials().emplace_back();
        pMesh->Materials().back().DiffuseMap() = m_Texture;

        meshPtrVec.push_back(pMesh);

        Entity::Init();
    }

    void GuiQuad::Update() {

    }

    void GuiQuad::CleanUp() {
        Entity::CleanUp();
    }
}