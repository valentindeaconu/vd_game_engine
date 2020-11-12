//
// Created by Vali on 8/29/2020.
//

#include "GuiQuad.hpp"

namespace mod::gui {
    GuiQuad::GuiQuad(const vd::model::Texture2DPtr& texture,
                     const glm::vec2& position,
                     const glm::vec2& scale)
       : m_Texture(texture)
   {
       this->LocalTransform().SetTranslation(position.x, position.y, 0.0f);
       this->LocalTransform().SetScaling(scale.x, scale.y, 1.0f);
    }

    GuiQuad::~GuiQuad() = default;

    void GuiQuad::Init() {
        vd::model::MeshPtrVec& meshPtrVec = Meshes();

        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        meshPtr->vertices = {
                { .Position = glm::vec3(-1.0f, 1.0f, 0.0f) },
                { .Position = glm::vec3(-1.0f, -1.0f, 0.0f) },
                { .Position = glm::vec3(1.0f, 1.0f, 0.0f) },
                { .Position = glm::vec3(1.0f, -1.0f, 0.0f) }
        };

        meshPtr->indices = { 0, 1, 2, 2, 1, 3 };

        meshPtr->materials.push_back({ .diffuseMap = m_Texture });

        meshPtrVec.push_back(meshPtr);

        Entity::Init();
    }

    void GuiQuad::Update() {

    }

    void GuiQuad::CleanUp() {
        Entity::CleanUp();
    }
}