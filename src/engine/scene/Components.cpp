//
// Created by vali on 4/15/21.
//

#include "Components.hpp"

namespace vd::scene {
    TransformComponent::TransformComponent(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation) {
        Transform.Translation() = translation;
        Transform.Scale() = scale;
        Transform.XAxisRotationAngle() = rotation.x;
        Transform.YAxisRotationAngle() = rotation.y;
        Transform.ZAxisRotationAngle() = rotation.z;
    }

    MeshComponent::MeshComponent(const vd::model::Mesh& mesh)
        : Mesh(mesh)
    {
    }
}