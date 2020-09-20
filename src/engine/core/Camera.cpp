#include "Camera.hpp"

namespace vd::core
{

    Camera::Camera(const InputHandlerPtr& inputHandlerPtr)
        : inputHandlerPtr(inputHandlerPtr)
    {
    }

    Camera::~Camera() = default;

    void Camera::update()
    {
        prevPosition = position;
        prevForward = forward;
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(position, position + forward, CameraConstants::kUpVector);
    }

    bool Camera::isCameraMoved() const
    {
        return prevPosition != position;
    }

    bool Camera::isCameraRotated() const
    {
        return prevForward != forward;
    }

    glm::vec3& Camera::getPosition()
    {
        return position;
    }

    const glm::vec3& Camera::getPosition() const
    {
        return position;
    }

    void Camera::setPosition(const glm::vec3& position)
    {
        this->position = position;
    }

    glm::vec3& Camera::getTarget()
    {
        return target;
    }

    const glm::vec3& Camera::getTarget() const
    {
        return target;
    }

    void Camera::setTarget(const glm::vec3& target)
    {
        this->target = target;
    }

    glm::vec3& Camera::getDirection() {
        return forward;
    }

    const glm::vec3& Camera::getDirection() const {
        return forward;
    }

    glm::vec3& Camera::getRightDirection() {
        return right;
    }

    const glm::vec3& Camera::getRightDirection() const {
        return right;
    }

    glm::vec3& Camera::getUpDirection() {
        return up;
    }

    const glm::vec3& Camera::getUpDirection() const {
        return up;
    }

    void Camera::updatePositionVectors()
    {
        right = glm::normalize(glm::cross(forward, CameraConstants::kUpVector));
        up = glm::normalize(glm::cross(right, forward));
    }

    float Camera::getPitch() const {
        // cos(pitch) = dot(up, forward) / (len(up) * len(forward))
        // up, forward - normalised => len(up) = len(forward) = 1
        // => pitch = acos(dot(up, forward))
        // acos returns radians
        // return glm::degrees(std::acos(glm::dot(up, forward)));


        return glm::degrees(-asin(dot(forward, glm::vec3(0.0f, 1.0f, 0.0f))));
    }

    float Camera::getYaw() const {
        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 z_unit = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 straight_forward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));

        float yaw = glm::degrees(std::acos(glm::dot(straight_forward, x_unit)));
        
        yaw = (glm::dot(straight_forward, z_unit) > 0.0f) ? (360.0f - yaw) : yaw;

        return yaw;
    }

    float Camera::getRoll() const {
        // cos(yaw) = dot(up, right) / (len(up) * len(right))
        // up, right - normalised => len(up) = len(right) = 1
        // => roll = acos(dot(up, right))
        // acos returns radians
        return glm::degrees(std::acos(glm::dot(up, right)));
    }


}