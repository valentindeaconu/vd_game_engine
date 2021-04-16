//
// Created by Vali on 4/13/2021.
//

#include "Scene.hpp"
#include "Entity.hpp"
#include "ScriptableEntity.hpp"

namespace vd::scene {

    Entity Scene::CreateEntity() {
        return { m_Registry.create(), this };
    }

    void Scene::OnInit() {
        m_Registry.view<NativeScriptComponent>().each([=, this](auto entity, auto& nativeScriptComponent) {
            nativeScriptComponent.InstantiateScript();
            nativeScriptComponent.Instance->m_Entity = Entity{ entity, this };
            nativeScriptComponent.Instance->OnCreate();
        });
    }

    void Scene::OnUpdate(Timestep ts) {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent) {
            nativeScriptComponent.Instance->OnUpdate(ts);
        });
    }
    
    void Scene::OnRender(const RenderingPassType& renderingPass) {
        auto group = m_Registry.group<TransformComponent, MeshComponent>();

        for (auto& entity : group) {
            auto&& [transformComponent, meshComponent] = group.get<TransformComponent, MeshComponent>(entity);

            // TODO: Submit to renderer
        }
    }
    
    void Scene::OnCleanUp() {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nativeScriptComponent) {
            nativeScriptComponent.Instance->OnDestroy();
        });
    }

}