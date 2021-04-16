//
// Created by Vali on 4/13/2021.
//

#ifndef VDGE_COMPONENTS_HPP
#define VDGE_COMPONENTS_HPP

#include <engine/math/Transform.hpp>
#include <engine/model/Mesh.hpp>

namespace vd::scene {

    struct TransformComponent {
        vd::math::Transform Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation);
    };

    struct MeshComponent {
        vd::model::Mesh Mesh;

        MeshComponent(const MeshComponent&) = default;
        MeshComponent(const vd::model::Mesh& mesh);
    };

    class ScriptableEntity;
    struct NativeScriptComponent {
        std::unique_ptr<ScriptableEntity> Instance;

        // Pointer to a function which instantiate the instance
        using InstantiateFunction = ScriptableEntity*(*)();
        InstantiateFunction InstantiateScript;

        using DestroyFunction = void(*)();
        DestroyFunction DestroyScript;

        template <typename T>
        void Bind() {
            InstantiateScript = []() { return std::make_unique<T>(); };
            DestroyScript = [](NativeScriptComponent* nativeScriptComponent) {
                nativeScriptComponent->Instance = nullptr;
            };
        }
    };
}

#endif //VDGE_COMPONENTS_HPP