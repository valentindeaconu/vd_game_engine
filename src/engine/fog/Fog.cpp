//
// Created by Vali on 11/25/2020.
//

#include "Fog.hpp"

namespace vd::fog {

    Fog::Fog(float density, float skyDensity, float gradient, const glm::vec3& color)
        : m_Density(density)
        , m_SkyDensity(skyDensity)
        , m_Gradient(gradient)
        , m_Color(color)
    {
    }

    Fog::~Fog() = default;

    float& Fog::Density() {
        return m_Density;
    }

    float& Fog::SkyDensity() {
        return m_SkyDensity;
    }

    float& Fog::Gradient() {
        return m_Gradient;
    }

    glm::vec3& Fog::Color() {
        return m_Color;
    }

}