#include "Transform.hpp"

namespace vd::math {

    Transform::Transform()
        : m_Translation(0.0f)
        , m_Scaling(1.0f)
        , m_AxisRotationAngle(0.0f)
        , m_AxisRotationAngleComputed(false, false, false)
        , m_TransformComputed(false)
        , m_Transform()
        , m_XAxisRotation()
        , m_YAxisRotation()
        , m_ZAxisRotation()
    {
        ComputeTransform();
    }

    glm::mat4 Transform::Get() const {
        ComputeTransform();

        return m_Transform;
    }

    glm::mat4 Transform::Inverse() const {
        ComputeTransform();

        return glm::inverse(m_Transform);
    }

    glm::mat4 Transform::TranslationMatrix() const {
        return glm::translate(glm::mat4(1.0f), m_Translation);
    }

    glm::mat4 Transform::ScaleMatrix() const {
        return glm::scale(glm::mat4(1.0f), m_Scaling);
    }

    glm::mat4 Transform::RotationMatrix() const {
        glm::mat4 rotX = glm::mat4_cast(m_XAxisRotation);
        glm::mat4 rotY = glm::mat4_cast(m_YAxisRotation);
        glm::mat4 rotZ = glm::mat4_cast(m_ZAxisRotation);

        return rotZ * rotY * rotX;
    }

    glm::vec3& Transform::Translation() {
        m_TransformComputed = false;

        return m_Translation;
    }

    glm::vec3& Transform::Scale() {
        m_TransformComputed = false;

        return m_Scaling;
    }

    float& Transform::XAxisRotationAngle() {
        ComputeXAxisRotationAngle();

        m_AxisRotationAngleComputed.x = false;
        m_TransformComputed = false;

        return m_AxisRotationAngle.x;
    }

    float& Transform::YAxisRotationAngle() {
        ComputeYAxisRotationAngle();

        m_AxisRotationAngleComputed.y = false;
        m_TransformComputed = false;

        return m_AxisRotationAngle.y;
    }

    float& Transform::ZAxisRotationAngle() {
        ComputeZAxisRotationAngle();

        m_AxisRotationAngleComputed.z = false;
        m_TransformComputed = false;

        return m_AxisRotationAngle.z;
    }

    glm::vec4 Transform::operator*(const glm::vec4& operand) const {
        return Get() * operand;
    }

    void Transform::ComputeTransform() const {
        auto nc_this = const_cast<Transform*>(this);

        nc_this->ComputeXAxisRotationAngle();
        nc_this->ComputeYAxisRotationAngle();
        nc_this->ComputeZAxisRotationAngle();

        if (!m_TransformComputed) {
            nc_this->m_Transform = TranslationMatrix() * ScaleMatrix() * RotationMatrix();
            nc_this->m_TransformComputed = true;
        }
    }

    void Transform::ComputeXAxisRotationAngle() {
        if (!m_AxisRotationAngleComputed.x) {
            m_AxisRotationAngle.x = glm::clamp(m_AxisRotationAngle.x, 0.0f, 360.0f);
            m_XAxisRotation = glm::angleAxis(glm::radians(m_AxisRotationAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));

            m_AxisRotationAngleComputed.x = true;
            m_TransformComputed = false;
        }
    }

    void Transform::ComputeYAxisRotationAngle() {
        if (!m_AxisRotationAngleComputed.y) {
            m_AxisRotationAngle.y = glm::clamp(m_AxisRotationAngle.y, 0.0f, 360.0f);
            m_YAxisRotation = glm::angleAxis(glm::radians(m_AxisRotationAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));

            m_AxisRotationAngleComputed.y = true;
            m_TransformComputed = false;
        }
    }

    void Transform::ComputeZAxisRotationAngle() {
        if (!m_AxisRotationAngleComputed.z) {
            m_AxisRotationAngle.z = glm::clamp(m_AxisRotationAngle.z, 0.0f, 360.0f);
            m_ZAxisRotation = glm::angleAxis(glm::radians(m_AxisRotationAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));

            m_AxisRotationAngleComputed.z = true;
            m_TransformComputed = false;
        }
    }
}