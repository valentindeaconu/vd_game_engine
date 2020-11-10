//
// Created by Vali on 8/29/2020.
//

#include "GuiQuad.hpp"

namespace mod::gui {

    GuiQuad::GuiQuad(const vd::EnginePtr &enginePtr,
                     const vd::model::Texture2DPtr& texture,
                     const glm::vec2& position,
                     const glm::vec2& scale)
       : Entity(enginePtr)
       , texture(texture)
   {
        this->getLocalTransform().setTranslation(position.x, position.y, 0.0f);
        this->getLocalTransform().setScaling(scale.x, scale.y, 1.0f);
    }

    GuiQuad::~GuiQuad() = default;

    void GuiQuad::init() {
        vd::model::MeshPtrVec& meshPtrVec = getMeshes();

        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        meshPtr->vertices = {
                { .Position = glm::vec3(-1.0f, 1.0f, 0.0f) },
                { .Position = glm::vec3(-1.0f, -1.0f, 0.0f) },
                { .Position = glm::vec3(1.0f, 1.0f, 0.0f) },
                { .Position = glm::vec3(1.0f, -1.0f, 0.0f) }
        };

        meshPtr->indices = { 0, 1, 2, 2, 1, 3 };

        meshPtr->materials.push_back({ .diffuseMap = texture });

        meshPtrVec.push_back(meshPtr);

        Entity::init();
    }

    void GuiQuad::update() {

    }

    void GuiQuad::cleanUp() {
        Entity::cleanUp();
    }
}