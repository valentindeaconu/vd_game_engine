#include "Camera.hpp"

#include <engine/camera/CameraWrapper.hpp>

#include <engine/camera/free/FreeCamera.hpp>
#include <engine/camera/third/ThirdPersonCamera.hpp>

namespace vd::camera {

    Camera::Camera()
        : m_Forward(0.0f)
        , m_Right(0.0f)
        , m_Up(0.0f)
        , m_Position(0.0f)
        , m_Target(0.0f)
        , m_PrevPosition(0.0f)
        , m_PrevForward(0.0f)
    {
    }

    Camera::~Camera() = default;

    glm::mat4 Camera::ViewMatrix() {
        return glm::lookAt(m_Position, m_Position + m_Forward, Camera::kUpVector);
    }

    bool Camera::CameraMoved() const {
        return m_PrevPosition != m_Position;
    }

    bool Camera::CameraRotated() const {
        return m_PrevForward != m_Forward;
    }

    glm::vec3& Camera::Position() {
        return m_Position;
    }

    glm::vec3& Camera::Target() {
        return m_Target;
    }

    const glm::vec3& Camera::Forward() const {
        return m_Forward;
    }

    const glm::vec3& Camera::Right() const {
        return m_Right;
    }

    const glm::vec3& Camera::Up() const {
        return m_Up;
    }

    float Camera::Pitch() const {
        // TODO: Compute pitch from position vectors

        // cos(pitch) = dot(up, forward) / (len(up) * len(forward))
        // up, forward - normalised => len(up) = len(forward) = 1
        // => pitch = acos(dot(up, forward))
        // acos returns radians
        // return glm::degrees(std::acos(glm::dot(up, forward)));


        return glm::degrees(-asin(dot(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f))));
    }

    float Camera::Yaw() const {
        // TODO: Compute yaw from position vectors

        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 z_unit = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 straight_forward = glm::normalize(glm::vec3(m_Forward.x, 0.0f, m_Forward.z));

        float yaw = glm::degrees(std::acos(glm::dot(straight_forward, x_unit)));
        
        yaw = (glm::dot(straight_forward, z_unit) > 0.0f) ? (360.0f - yaw) : yaw;

        return yaw;
    }

    float Camera::Roll() const {
        // TODO: Compute roll from position vectors

        // cos(yaw) = dot(up, right) / (len(up) * len(right))
        // up, right - normalised => len(up) = len(right) = 1
        // => roll = acos(dot(up, right))
        // acos returns radians
        return glm::degrees(std::acos(glm::dot(m_Up, m_Right)));
    }

    void Camera::UpdatePositionVectors() {
        m_Right = glm::normalize(glm::cross(m_Forward, Camera::kUpVector));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }

    CameraManager::CameraManager()
        : m_CameraMode(eThirdPerson)
    {
        m_pFreeCamera = std::make_shared<impl::FreeCamera>("./resources/properties/camera.properties");

        m_pFreeCamera->Speed() = 4.0f;
        m_pFreeCamera->RotationSensitivity() = 0.5f;

        m_pThirdPersonCamera = std::make_shared<impl::ThirdPersonCamera>();

        m_pThirdPersonCamera->Pitch() = 20.0f;
        m_pThirdPersonCamera->DistanceFromPlayer() = 8.5f;
        m_pThirdPersonCamera->AngleAroundPlayer() = 180.0f;

        m_pCameraWrapper = std::make_shared<wrapper::CameraWrapper>();

        m_pCameraWrapper->Camera() = m_pThirdPersonCamera;

        vd::ObjectOfType<camera::Camera>::Provide(m_pCameraWrapper);
    }

    CameraManager::~CameraManager() = default;

    void CameraManager::Link() {
        m_pEventHandler = vd::ObjectOfType<event::EventHandler>::Find();
    }

    void CameraManager::Init() {

    }

    void CameraManager::Update() {
        if (m_pEventHandler->KeyDown(GLFW_KEY_C)) {
            if (m_CameraMode == eFree) {
                m_CameraMode = eThirdPerson;
                m_pCameraWrapper->Camera() = m_pThirdPersonCamera;
                Logger::log("3rd Person Camera Mode");
            } else {
                m_CameraMode = eFree;
                m_pCameraWrapper->Camera() = m_pFreeCamera;
                Logger::log("Free Camera Mode");
            }
        }

        switch (m_CameraMode) {
            case eFree: UpdateFreeCamera(); break;
            case eFirstPerson: UpdateFirstPersonCamera(); break;
            case eThirdPerson: UpdateThirdPersonCamera(); break;
        }
    }

    void CameraManager::CleanUp() {

    }

    const CameraManager::CameraMode& CameraManager::Mode() const {
        return m_CameraMode;
    }

    void CameraManager::UpdateFreeCamera() {
        m_pFreeCamera->m_PrevPosition = m_pFreeCamera->m_Position;
        m_pFreeCamera->m_PrevForward = m_pFreeCamera->m_Forward;

        float movAmt = m_pFreeCamera->m_Speed;
        float rotAmt = m_pFreeCamera->m_Speed * m_pFreeCamera->m_RotationSensitivity;

        if (m_pEventHandler->KeyHolding(GLFW_KEY_W))
            m_pFreeCamera->Move(m_pFreeCamera->m_Forward, movAmt);

        if (m_pEventHandler->KeyHolding(GLFW_KEY_S))
            m_pFreeCamera->Move(m_pFreeCamera->m_Forward, -movAmt);

        if (m_pEventHandler->KeyHolding(GLFW_KEY_A))
            m_pFreeCamera->Move(m_pFreeCamera->m_Right, -movAmt);

        if (m_pEventHandler->KeyHolding(GLFW_KEY_D))
            m_pFreeCamera->Move(m_pFreeCamera->m_Right, movAmt);

        if (m_pEventHandler->KeyHolding(GLFW_KEY_T))
            m_pFreeCamera->Move(m_pFreeCamera->m_Up, movAmt);

        if (m_pEventHandler->KeyHolding(GLFW_KEY_G))
            m_pFreeCamera->Move(m_pFreeCamera->m_Up, -movAmt);

        if (m_pEventHandler->ButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (m_pEventHandler->MouseMoved()) {
                float yawChange = (float) m_pEventHandler->MouseDX() * rotAmt;
                m_pFreeCamera->Rotate(0.0f, -yawChange);
            }
        }

        if (m_pEventHandler->ButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (m_pEventHandler->MouseMoved()) {
                float pitchChange = (float) m_pEventHandler->MouseDY() * rotAmt;
                m_pFreeCamera->Rotate(-pitchChange, 0.0f);
            }
        }

        if (m_pEventHandler->KeyHolding(GLFW_KEY_K)) {
            Logger::log("Position (" +
                        std::to_string(m_pFreeCamera->m_Position.x) + ", " +
                        std::to_string(m_pFreeCamera->m_Position.y) + ", " +
                        std::to_string(m_pFreeCamera->m_Position.z) + ")"
            );
        }
    }

    void CameraManager::UpdateFirstPersonCamera() {

    }

    void CameraManager::UpdateThirdPersonCamera() {
        m_pThirdPersonCamera->m_PrevPosition = m_pThirdPersonCamera->m_Position;
        m_pThirdPersonCamera->m_PrevForward = m_pThirdPersonCamera->m_Forward;

        if (m_pEventHandler->MouseScrolled()) {
            auto zoomLevel = (float) m_pEventHandler->MouseDWheel();
            float distanceFromPlayer = m_pThirdPersonCamera->DistanceFromPlayer() - zoomLevel;
            distanceFromPlayer = glm::clamp(distanceFromPlayer, 2.5f, 35.0f);
            m_pThirdPersonCamera->DistanceFromPlayer() = distanceFromPlayer;
        }

        if (m_pEventHandler->ButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE)) {
            if (m_pEventHandler->MouseMoved()) {
                auto pitchChange = (float) m_pEventHandler->MouseDY();
                float pitch = m_pThirdPersonCamera->Pitch() - pitchChange;
                pitch = glm::clamp(pitch, 5.0f, 75.0f);
                m_pThirdPersonCamera->Pitch() = pitch;
            }
        }

        if (m_pEventHandler->ButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE)) {
            if (m_pEventHandler->MouseMoved()) {
                auto angleChange = (float) m_pEventHandler->MouseDX();
                m_pThirdPersonCamera->AngleAroundPlayer() -= angleChange * 2.5f;
            }
        }

        m_pThirdPersonCamera->Update();
    }

}