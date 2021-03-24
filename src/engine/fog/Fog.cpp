//
// Created by Vali on 11/25/2020.
//

#include "Fog.hpp"

namespace vd::fog {

    Fog::Fog(float density, float gradient, const glm::vec3& color)
        : m_Density(density)
        , m_Gradient(gradient)
        , m_Color(color)
    {
    }

    float& Fog::Density() {
        return m_Density;
    }

    float& Fog::Gradient() {
        return m_Gradient;
    }

    glm::vec3& Fog::Color() {
        return m_Color;
    }

}