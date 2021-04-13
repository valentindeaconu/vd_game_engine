//
// Created by Vali on 4/13/2021.
//

#include "Entity.hpp"

namespace vd::scene {

    Entity::Entity() 
        : m_Handle(entt::null)
        , m_Scene(nullptr)
    {
    }

    Entity::Entity(entt::entity handle, Scene* scene) 
        : m_Handle(handle)
        , m_Scene(scene) 
    {
    }

}