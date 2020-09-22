#include "FreeCamera.hpp"

namespace vd::core::impl
{
    FreeCamera::FreeCamera(const InputHandlerPtr& inputHandlerPtr)
        : Camera(inputHandlerPtr)
        , pitch(0.0f)
        , yaw(0.0f)
        , speed(0.0f)
    {
    }

    FreeCamera::~FreeCamera() = default;

    void FreeCamera::init(CameraInitParametersPtr parameters) {
        auto* params = (FreeCameraInitParameters*)(parameters);

        position = params->initPosition;
        target = params->initTarget;
        speed = params->speed;

        forward = glm::normalize(target - position);
        updatePositionVectors();

        const glm::vec3 x_unit(1.0f, 0.0f, 0.0f);
        const glm::vec3 y_unit(0.0f, 1.0f, 0.0f);
        const glm::vec3 z_unit(0.0f, 0.0f, 1.0f);

        glm::vec3 fwrd = forward;

        pitch = -glm::degrees(-glm::asin(glm::dot(fwrd, y_unit)));
        fwrd.y = 0.0f;
        fwrd = glm::normalize(fwrd);

        yaw = -glm::degrees(glm::acos(glm::dot(fwrd, x_unit)));

        if (glm::dot(fwrd, z_unit) > 0) {
            yaw = 360 - yaw;
        }
    }

    void FreeCamera::update() {
        Camera::update();

        float movAmt = speed;
        float rotAmt = speed * 0.5f;

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_W))
            move(forward, movAmt);

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_S))
            move(forward, -movAmt);

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_A))
            move(right, -movAmt);

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_D))
            move(right, movAmt);

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_T))
            move(up, movAmt);

        if (inputHandlerPtr->getKeyHolding(GLFW_KEY_G))
            move(up, -movAmt);

        if (inputHandlerPtr->getButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (inputHandlerPtr->getMouseMoved()) {
                float yawChange = (float) inputHandlerPtr->getMouseDX() * rotAmt;
                rotate(0.0f, -yawChange);
            }
        }

        if (inputHandlerPtr->getButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (inputHandlerPtr->getMouseMoved()) {
                float pitchChange = (float) inputHandlerPtr->getMouseDY() * rotAmt;
                rotate(-pitchChange, 0.0f);
            }
        }
    }

    void FreeCamera::reflect(float yAxisSymmetric) {
        float offset = 2.0f * std::abs(position.y - yAxisSymmetric);
        if (position.y > yAxisSymmetric) {
            position.y -= offset;
        } else {
            position.y += offset;
        }

        forward.y = -forward.y;
        updatePositionVectors();
    }

    void FreeCamera::move(const glm::vec3& dir, float amount)
    {
        position += dir * amount;
    }

    void FreeCamera::rotate(float pitch, float yaw)
    {
        this->pitch = std::clamp(this->pitch + pitch, -89.0f, 89.0f);
        this->yaw += yaw;

        updateForwardVector();
        updatePositionVectors();
    }

    void FreeCamera::updateForwardVector() {
        forward.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
        forward.y = std::sin(glm::radians(this->pitch));
        forward.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
        forward = glm::normalize(forward);
    }
}