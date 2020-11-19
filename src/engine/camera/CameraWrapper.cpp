//
// Created by Vali on 11/18/2020.
//

#include "CameraWrapper.hpp"

namespace vd::camera::wrapper {

    CameraPtr& CameraWrapper::Camera() {
        return m_pImpl;
    }

    void CameraWrapper::Reflect(const Camera::Axis& axis, float amount) {
        m_pImpl->Reflect(axis, amount);
    }

    glm::mat4 CameraWrapper::ViewMatrix() {
        return m_pImpl->ViewMatrix();
    }

    bool CameraWrapper::CameraMoved() const {
        return m_pImpl->CameraMoved();
    }

    bool CameraWrapper::CameraRotated() const {
        return m_pImpl->CameraRotated();
    }

    glm::vec3& CameraWrapper::Position() {
        return m_pImpl->Position();
    }

    glm::vec3& CameraWrapper::Target() {
        return m_pImpl->Target();
    }

    const glm::vec3& CameraWrapper::Forward() const {
        return m_pImpl->Forward();
    }

    const glm::vec3& CameraWrapper::Right() const {
        return m_pImpl->Right();
    }

    const glm::vec3& CameraWrapper::Up() const {
        return m_pImpl->Up();
    }

    float CameraWrapper::Pitch() const {
        return m_pImpl->Pitch();
    }

    float CameraWrapper::Yaw() const {
        return m_pImpl->Yaw();
    }

    float CameraWrapper::Roll() const {
        return m_pImpl->Roll();
    }

}