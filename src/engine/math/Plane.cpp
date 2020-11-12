//
// Created by Vali on 11/11/2020.
//

#include "Plane.hpp"

namespace vd::math {
    Plane::Plane()
        : n(0.0f)
        , d(0.0f)
    {
    }

    Plane::Plane(const glm::vec3& n, float d)
        : n(n)
        , d(d)
    {
    }

    Plane::Plane(const glm::vec3& bl, const glm::vec3& br, const glm::vec3& tl, const glm::vec3& tr)
        : n(0.0f)
        , d(0.0f)
    {
        glm::vec3 bl_to_br = br - bl;
        glm::vec3 bl_to_tl = tl - bl;

        this->n = glm::normalize(glm::cross(bl_to_br, bl_to_tl));
        this->d = -(this->n.x * tr.x + this->n.y * tr.y + this->n.z * tr.z);
    }
}