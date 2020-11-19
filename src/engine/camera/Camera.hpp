#ifndef __CAMERA_HPP_
#define __CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

#include <engine/component/IManager.hpp>

#include <engine/injector/Injectable.hpp>
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

        virtual glm::mat4 ViewMatrix();

        [[nodiscard]] virtual bool CameraMoved() const;
        [[nodiscard]] virtual bool CameraRotated() const;

        virtual glm::vec3& Position();

        virtual glm::vec3& Target();

        [[nodiscard]] virtual const glm::vec3& Forward() const;
        [[nodiscard]] virtual const glm::vec3& Right() const;
        [[nodiscard]] virtual const glm::vec3& Up() const;

        [[nodiscard]] virtual float Pitch() const;
        [[nodiscard]] virtual float Yaw() const;
        [[nodiscard]] virtual float Roll() const;

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

    /// Forward declarations for Camera implementations
    namespace impl {
        class FreeCamera;
        class FirstPersonCamera;
        class ThirdPersonCamera;

        typedef std::shared_ptr<FreeCamera>         FreeCameraPtr;
        typedef std::shared_ptr<FirstPersonCamera>  FirstPersonCameraPtr;
        typedef std::shared_ptr<ThirdPersonCamera>  ThirdPersonCameraPtr;
    }

    namespace wrapper {
        class CameraWrapper;

        typedef std::shared_ptr<CameraWrapper>      CameraWrapperPtr;
    }

    class CameraManager : public component::IManager, public injector::Injectable {
    public:
        enum CameraMode {
            eFree = 0,
            eFirstPerson,
            eThirdPerson
        };

        CameraManager();
        ~CameraManager();

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const CameraMode& Mode() const;
    private:
        void UpdateFreeCamera();
        void UpdateFirstPersonCamera();
        void UpdateThirdPersonCamera();

        impl::FreeCameraPtr m_pFreeCamera;
        impl::FirstPersonCameraPtr m_pFirstPersonCamera;
        impl::ThirdPersonCameraPtr m_pThirdPersonCamera;

        wrapper::CameraWrapperPtr m_pCameraWrapper;

        event::EventHandlerPtr m_pEventHandler;

        CameraMode m_CameraMode;
    };

    typedef std::shared_ptr<CameraManager>  CameraManagerPtr;
}

#endif // !__CAMERA_HPP_
