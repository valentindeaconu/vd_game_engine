#include "Transform.hpp"

namespace vd::math {

    Transform::Transform()
        : m_Translation(0.0f)
        , m_Scaling(1.0f)
        , m_TransformComputed(false)
    {
        SetXRotationAngle(0.0f);
        SetYRotationAngle(0.0f);
        SetZRotationAngle(0.0f);
    }

    glm::mat4 Transform::Get() const {
        ComputeTransform();
        return m_Transform;
    }

    glm::mat4 Transform::Inverse() const {
        ComputeTransform();
        return glm::inverse(m_Transform);
    }

    glm::mat4 Transform::GetTranslation() const {
        return glm::translate(glm::mat4(1.0f), m_Translation);
    }

    glm::mat4 Transform::GetRotation() const {
        glm::mat4 rotX = glm::mat4_cast(m_XAxisRotation);
        glm::mat4 rotY = glm::mat4_cast(m_YAxisRotation);
        glm::mat4 rotZ = glm::mat4_cast(m_ZAxisRotation);

        return rotZ * rotY * rotX;
    }

    glm::mat4 Transform::GetScaling() const {
        return glm::scale(glm::mat4(1.0f), m_Scaling);
    }

    void Transform::SetTranslation(float x, float y, float z) {
        this->m_Translation.x = x;
        this->m_Translation.y = y;
        this->m_Translation.z = z;

        this->m_TransformComputed = false;
    }

    void Transform::SetTranslation(const glm::vec3& translation) {
        SetTranslation(translation.x, translation.y, translation.z);
    }

    const glm::vec3& Transform::GetTranslationVector() const {
        return m_Translation;
    }

    void Transform::SetScaling(float x, float y, float z) {
        this->m_Scaling.x = x;
        this->m_Scaling.y = y;
        this->m_Scaling.z = z;

        this->m_TransformComputed = false;
    }

    void Transform::SetScaling(const glm::vec3& scaling) {
        SetScaling(scaling.x, scaling.y, scaling.z);
    }

    const glm::vec3& Transform::GetScalingVector() const {
        return m_Scaling;
    }

    void Transform::SetXRotationAngle(float angle) {
        angle = glm::clamp(angle, 0.0f, 360.0f);
        m_XAxisRotationAngle = angle;
        m_XAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        this->m_TransformComputed = false;
    }

    float Transform::GetXAxisRotationAngle() const {
        return m_XAxisRotationAngle;
    }

    const glm::quat& Transform::GetXAxisRotation() const {
        return m_XAxisRotation;
    }

    float Transform::GetYAxisRotationAngle() const {
        return m_YAxisRotationAngle;
    }

    void Transform::SetYRotationAngle(float angle) {
        angle = glm::clamp(angle, 0.0f, 360.0f);
        m_YAxisRotationAngle = angle;
        m_YAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        this->m_TransformComputed = false;
    }

    const glm::quat& Transform::GetYAxisRotation() const {
        return m_YAxisRotation;
    }

    void Transform::SetZRotationAngle(float angle) {
        angle = glm::clamp(angle, 0.0f, 360.0f);
        m_ZAxisRotationAngle = angle;
        m_ZAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        this->m_TransformComputed = false;
    }

    float Transform::GetZAxisRotationAngle() const {
        return m_ZAxisRotationAngle;
    }

    const glm::quat& Transform::GetZAxisRotation() const {
        return m_ZAxisRotation;
    }

    glm::vec4 Transform::operator*(const glm::vec4& operand) const {
        return Get() * operand;
    }

    void Transform::ComputeTransform() const {
        if (!m_TransformComputed) {
            auto nc_this = const_cast<Transform*>(this);
            nc_this->m_Transform = GetTranslation() * GetScaling() * GetRotation();
            nc_this->m_TransformComputed = true;
        }
    }
}