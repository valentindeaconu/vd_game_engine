#include "Light.hpp"

namespace vd::light {
    Light::Light(const LightType& type,
                 const glm::vec3& position,
                 const glm::vec3& color,
                 const glm::vec3& attenuation,
                 float ambientStrength,
                 float specularStrength,
                 float shininess)
        : m_kType(type)
        , m_Position(position)
        , m_Color(color)
        , m_Attenuation(attenuation)
        , m_AmbientStrength(ambientStrength)
        , m_SpecularStrength(specularStrength)
        , m_Shininess(shininess)
    {
    }

    Light::~Light() = default;

    const LightType& Light::Type() const {
        return m_kType;
    }

    glm::vec3& Light::Position() {
        return m_Position;
    }

    glm::vec3& Light::EyeSpacePosition() {
        return m_EyeSpacePosition;
    }

    glm::vec3& Light::Color() {
        return m_Color;
    }

    glm::vec3& Light::Attenuation() {
        return m_Attenuation;
    }

    float& Light::AmbientStrength() {
        return m_AmbientStrength;
    }

    float& Light::SpecularStrength() {
        return m_SpecularStrength;
    }

    float& Light::Shininess() {
        return m_Shininess;
    }

}