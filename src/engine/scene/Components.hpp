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
        TransformComponent(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation) 
        {
            Transform.Translation() = translation;
            Transform.Scale() = scale;
            Transform.XAxisRotationAngle() = rotation.x;
            Transform.YAxisRotationAngle() = rotation.y;
            Transform.ZAxisRotationAngle() = rotation.z;
        }
    };

}

#endif //VDGE_COMPONENTS_HPP