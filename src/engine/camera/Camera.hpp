#ifndef __CAMERA_HPP_
#define __CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

#include <engine/component/IManager.hpp>

#include <engine/event/EventHandler.hpp>

namespace vd::camera {
    class CameraManager;

    class Camera {
    public:
        constexpr static const glm::vec3 kUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

        enum Axis {
            eX = 0,
            eY,
            eZ
        };

        Camera();
        ~Camera();

        virtual void Reflect(const Axis& axis, float amount) = 0;

        glm::mat4 ViewMatrix();

        [[nodiscard]] bool CameraMoved() const;
        [[nodiscard]] bool CameraRotated() const;

        glm::vec3& Position();

        glm::vec3& Target();

        [[nodiscard]] const glm::vec3& Forward() const;
        [[nodiscard]] const glm::vec3& Right() const;
        [[nodiscard]] const glm::vec3& Up() const;

        [[nodiscard]] float Pitch() const;
        [[nodiscard]] float Yaw() const;
        [[nodiscard]] float Roll() const;

    protected:
        void UpdatePositionVectors();

        glm::vec3 m_Forward;
        glm::vec3 m_Right;
        glm::vec3 m_Up;

        glm::vec3 m_Position;
        glm::vec3 m_Target;

    private:
        friend class CameraManager;

        glm::vec3 m_PrevPosition;
        glm::vec3 m_PrevForward;
    };
    typedef std::shared_ptr<Camera>	CameraPtr;

    /// Forward declarations for ICamera implementations
    namespace impl {
        class FreeCamera;
        class FirstPersonCamera;
        class ThirdPersonCamera;

        typedef std::shared_ptr<FreeCamera>         FreeCameraPtr;
        typedef std::shared_ptr<FirstPersonCamera>  FirstPersonCameraPtr;
        typedef std::shared_ptr<ThirdPersonCamera>  ThirdPersonCameraPtr;

    }

    class CameraManager : public vd::component::IManager {
    public:
        enum CameraMode {
            eFree = 0,
            eFirstPerson,
            eThirdPerson
        };

        CameraManager();
        ~CameraManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const CameraMode& Mode() const;
    private:
        void UpdateFreeCamera();
        void UpdateFirstPersonCamera();
        void UpdateThirdPersonCamera();

        impl::FreeCameraPtr m_FreeCameraPtr;
        impl::FirstPersonCameraPtr m_FirstPersonCameraPtr;
        impl::ThirdPersonCameraPtr m_ThirdPersonCameraPtr;

        event::EventHandlerPtr m_EventHandlerPtr;

        CameraMode m_CameraMode;
    };

    typedef std::shared_ptr<CameraManager>  CameraManagerPtr;
}

#endif // !__CAMERA_HPP_
