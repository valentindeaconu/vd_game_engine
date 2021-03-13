//
// Created by Vali on 11/12/2020.
//

#include "FreeCamera.hpp"

namespace vd::camera::impl {
    FreeCamera::FreeCamera(const std::string& propsFilePath)
        : Camera()
        , m_RotationSensitivity(0.5f)
        , m_Pitch(0.0f)
        , m_Yaw(0.0f)
    {
        auto pProperties = vd::loader::PropertiesLoader::Load(propsFilePath);

        m_Position = pProperties->Get<glm::vec3>("Camera.Position");
        m_Target = pProperties->Get<glm::vec3>("Camera.Target");
        m_Speed = pProperties->Get<float>("Camera.Speed");

        m_Forward = glm::normalize(m_Target - m_Position);
        UpdatePositionVectors();

        const glm::vec3 x_unit(1.0f, 0.0f, 0.0f);
        const glm::vec3 y_unit(0.0f, 1.0f, 0.0f);
        const glm::vec3 z_unit(0.0f, 0.0f, 1.0f);

        glm::vec3 forward = m_Forward;

        m_Pitch = -glm::degrees(-glm::asin(glm::dot(forward, y_unit)));
        forward.y = 0.0f;
        forward = glm::normalize(forward);

        m_Yaw = -glm::degrees(glm::acos(glm::dot(forward, x_unit)));

        if (glm::dot(forward, z_unit) > 0) {
            m_Yaw = 360 - m_Yaw;
        }
    }

    void FreeCamera::Reflect(const Axis& axis, float amount) {
        switch (axis) {
            case eX: {
                float offset = 2.0f * std::abs(m_Position.x - amount);
                if (m_Position.x > amount) {
                    m_Position.x -= offset;
                } else {
                    m_Position.x += offset;
                }

                m_Forward.x = -m_Forward.x;
                UpdatePositionVectors();
                break;
            }
            case eY: {
                float offset = 2.0f * std::abs(m_Position.y - amount);
                if (m_Position.y > amount) {
                    m_Position.y -= offset;
                } else {
                    m_Position.y += offset;
                }

                m_Forward.y = -m_Forward.y;
                UpdatePositionVectors();
                break;
            }
            case eZ: {
                float offset = 2.0f * std::abs(m_Position.z - amount);
                if (m_Position.z > amount) {
                    m_Position.z -= offset;
                } else {
                    m_Position.z += offset;
                }

                m_Forward.z = -m_Forward.z;
                UpdatePositionVectors();
                break;
            }
        }
    }

    float& FreeCamera::Speed() {
        return m_Speed;
    }

    float& FreeCamera::RotationSensitivity() {
        return m_RotationSensitivity;
    }

    void FreeCamera::Move(const glm::vec3& direction, float amount) {
        m_Position += direction * amount;
    }

    void FreeCamera::Rotate(float pitch, float yaw) {
        this->m_Pitch = std::clamp(this->m_Pitch + pitch, -89.0f, 89.0f);
        this->m_Yaw += yaw;

        UpdateForwardVector();
        UpdatePositionVectors();
    }

    void FreeCamera::UpdateForwardVector() {
        m_Forward.x = std::cos(glm::radians(this->m_Yaw)) * std::cos(glm::radians(this->m_Pitch));
        m_Forward.y = std::sin(glm::radians(this->m_Pitch));
        m_Forward.z = std::sin(glm::radians(this->m_Yaw)) * std::cos(glm::radians(this->m_Pitch));
        m_Forward = glm::normalize(m_Forward);
    }

}