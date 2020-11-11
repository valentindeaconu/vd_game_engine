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

    const LightType& Light::GetType() const {
        return m_kType;
    }

    glm::vec3& Light::GetPosition() {
        return m_Position;
    }

    const glm::vec3& Light::GetPosition() const {
        return m_Position;
    }

    void Light::SetPosition(const glm::vec3& position) {
        this->m_Position = position;
    }

    glm::vec3& Light::GetDirection() {
        return GetPosition();
    }

    const glm::vec3& Light::GetDirection() const {
        return GetPosition();
    }

    void Light::SetDirection(const glm::vec3& direction) {
        SetPosition(direction);
    }

    glm::vec3& Light::GetColor() {
        return m_Color;
    }

    const glm::vec3& Light::GetColor() const {
        return m_Color;
    }

    void Light::SetColor(const glm::vec3& color) {
        this->m_Color = color;
    }

    glm::vec3& Light::GetAttenuation() {
        return m_Attenuation;
    }

    const glm::vec3& Light::GetAttenuation() const {
        return m_Attenuation;
    }

    void Light::SetAttenuation(const glm::vec3& attenuation) {
        this->m_Attenuation = attenuation;
    }

    float& Light::GetAmbientStrength() {
        return m_AmbientStrength;
    }

    const float& Light::GetAmbientStrength() const {
        return m_AmbientStrength;
    }

    void Light::SetAmbientStrength(const float& ambientStrength) {
        this->m_AmbientStrength = ambientStrength;
    }

    float& Light::GetSpecularStrength() {
        return m_SpecularStrength;
    }

    const float& Light::GetSpecularStrength() const {
        return m_SpecularStrength;
    }

    void Light::SetSpecularStrength(const float& specularStrength) {
        this->m_SpecularStrength = specularStrength;
    }

    float& Light::GetShininess() {
        return m_Shininess;
    }

    const float& Light::GetShininess() const {
        return m_Shininess;
    }

    void Light::SetShininess(const float& shininess) {
        this->m_Shininess = shininess;
    }
}