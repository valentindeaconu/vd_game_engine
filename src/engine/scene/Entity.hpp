//
// Created by Vali on 4/13/2021.
//

#ifndef VDGE_ENTITY_HPP
#define VDGE_ENTITY_HPP

#include <entt.hpp>
#include <engine/exception/Exceptions.hpp>

#include "Scene.hpp"

namespace vd::scene {

    class Entity {
    public:
        Entity();
        Entity(entt::entity handle, Scene* scene);

        template <typename T>
        bool HasComponent() {
            return m_Scene->m_Registry.any_of<T>(m_Handle);
        }

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            if (HasComponent<T>()) {
                throw RuntimeError("Entity already has component!");
            }

            return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& GetComponent() {
            if (!HasComponent<T>()) {
                throw RuntimeError("Entity does not have component!");
            }

            return m_Scene->m_Registry.get<T>(m_Handle);
        }

        template <typename T>
        void RemoveComponent() {
            if (!HasComponent<T>()) {
                throw RuntimeError("Entity does not have component!");
            }

            m_Scene->m_Registry.remove<T>(m_Handle);
        }

    private:
        entt::entity    m_Handle;
        Scene*          m_Scene;
    };

}

#endif //VDGE_ENTITY_HPP