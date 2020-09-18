#ifndef __FREE_CAMERA_HPP_
#define __FREE_CAMERA_HPP_

#include "../Camera.hpp"

namespace vd::core
{
    struct FreeCameraInitParameters
    {
        glm::vec3 initPosition;
        glm::vec3 initTarget;
        float speed;
    };
}

namespace vd::core::impl
{
    class FreeCamera : public Camera
    {
    public:
        FreeCamera(const InputHandlerPtr& inputHandlerPtr);
        ~FreeCamera();

        void init(CameraInitParametersPtr parameters) override;
        void update() override;

        void invertPitch() override;

    private:
        void move(const glm::vec3& dir, float amount);
        void rotate(float pitch, float yaw);

        void updateForwardVector();

        float speed;

        float pitch;
        float yaw;
    };
}

#endif // !__CAMERA_HPP_