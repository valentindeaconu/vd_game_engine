#ifndef __CAMERA_HPP_
#define __CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "InputHandler.hpp"

namespace vd::core
{
    typedef void*	CameraInitParametersPtr;

    class Camera
    {
    public:
        explicit Camera(const InputHandlerPtr& inputHandlerPtr);
        ~Camera();

        virtual void init(CameraInitParametersPtr parameters) = 0;
        virtual void update();

        glm::mat4 getViewMatrix();

        [[nodiscard]] bool isCameraMoved() const;
        [[nodiscard]] bool isCameraRotated() const;

        glm::vec3& getPosition();
        [[nodiscard]] const glm::vec3& getPosition() const;
        void setPosition(const glm::vec3& position);

        glm::vec3& getTarget();
        [[nodiscard]] const glm::vec3& getTarget() const;
        void setTarget(const glm::vec3& target);

        glm::vec3& getDirection();
        [[nodiscard]] const glm::vec3& getDirection() const;

        glm::vec3& getRightDirection();
        [[nodiscard]] const glm::vec3& getRightDirection() const;

        glm::vec3& getUpDirection();
        [[nodiscard]] const glm::vec3& getUpDirection() const;

        [[nodiscard]] float getPitch() const;
        [[nodiscard]] float getYaw() const;
        [[nodiscard]] float getRoll() const;

        struct CameraConstants {
            constexpr static const glm::vec3 kUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
        };

    protected:
        void updatePositionVectors();

        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        glm::vec3 position;
        glm::vec3 target;

        InputHandlerPtr inputHandlerPtr;

    private:
        glm::vec3 prevPosition;
        glm::vec3 prevForward;
    };
    typedef std::shared_ptr<Camera>	CameraPtr;
}

#endif // !__CAMERA_HPP_
