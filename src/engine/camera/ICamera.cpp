#include "ICamera.hpp"

#include <engine/camera/free/FreeCamera.hpp>
#include <engine/camera/third/ThirdPersonCamera.hpp>

namespace vd::camera {

    ICamera::ICamera()
        : m_Forward(0.0f)
        , m_Right(0.0f)
        , m_Up(0.0f)
        , m_Position(0.0f)
        , m_Target(0.0f)
        , m_PrevPosition(0.0f)
        , m_PrevForward(0.0f)
    {
    }

    ICamera::~ICamera() = default;

    glm::mat4 ICamera::ViewMatrix() {
        return glm::lookAt(m_Position, m_Position + m_Forward, ICamera::kUpVector);
    }

    bool ICamera::CameraMoved() const {
        return m_PrevPosition != m_Position;
    }

    bool ICamera::CameraRotated() const {
        return m_PrevForward != m_Forward;
    }

    glm::vec3& ICamera::Position() {
        return m_Position;
    }

    glm::vec3& ICamera::Target() {
        return m_Target;
    }

    const glm::vec3& ICamera::Forward() const {
        return m_Forward;
    }

    const glm::vec3& ICamera::Right() const {
        return m_Right;
    }

    const glm::vec3& ICamera::Up() const {
        return m_Up;
    }

    float ICamera::Pitch() const {
        // TODO: Compute pitch from position vectors

        // cos(pitch) = dot(up, forward) / (len(up) * len(forward))
        // up, forward - normalised => len(up) = len(forward) = 1
        // => pitch = acos(dot(up, forward))
        // acos returns radians
        // return glm::degrees(std::acos(glm::dot(up, forward)));


        return glm::degrees(-asin(dot(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f))));
    }

    float ICamera::Yaw() const {
        // TODO: Compute yaw from position vectors

        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 z_unit = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 straight_forward = glm::normalize(glm::vec3(m_Forward.x, 0.0f, m_Forward.z));

        float yaw = glm::degrees(std::acos(glm::dot(straight_forward, x_unit)));
        
        yaw = (glm::dot(straight_forward, z_unit) > 0.0f) ? (360.0f - yaw) : yaw;

        return yaw;
    }

    float ICamera::Roll() const {
        // TODO: Compute roll from position vectors

        // cos(yaw) = dot(up, right) / (len(up) * len(right))
        // up, right - normalised => len(up) = len(right) = 1
        // => roll = acos(dot(up, right))
        // acos returns radians
        return glm::degrees(std::acos(glm::dot(m_Up, m_Right)));
    }

    void ICamera::UpdatePositionVectors() {
        m_Right = glm::normalize(glm::cross(m_Forward, ICamera::kUpVector));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }

    CameraManager::CameraManager()
        : m_CameraMode(eThirdPerson)
    {
        m_FreeCameraPtr = std::make_shared<impl::FreeCamera>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(100.0f, 1.0f, 100.0f));

        m_FreeCameraPtr->Speed() = 4.0f;
        m_FreeCameraPtr->RotationSensitivity() = 0.5f;

        m_ThirdPersonCameraPtr = std::make_shared<impl::ThirdPersonCamera>();

        m_ThirdPersonCameraPtr->Pitch() = 20.0f;
        m_ThirdPersonCameraPtr->DistanceFromPlayer() = 8.5f;
        m_ThirdPersonCameraPtr->AngleAroundPlayer() = 180.0f;

        vd::ObjectOfType<camera::ICamera>::Provide(m_ThirdPersonCameraPtr);
    }

    CameraManager::~CameraManager() = default;

    void CameraManager::Init() {
        m_EventHandlerPtr = vd::ObjectOfType<event::EventHandler>::Find();

        // Link Camera to Player and Terrain
        m_ThirdPersonCameraPtr->Link();
    }

    void CameraManager::Update() {
        if (m_EventHandlerPtr->KeyDown(GLFW_KEY_C)) {
            if (m_CameraMode == eFree) {
                m_CameraMode = eThirdPerson;
                ObjectOfType<camera::ICamera>::Provide(std::dynamic_pointer_cast<camera::ICamera>(m_ThirdPersonCameraPtr));
                logger::Logger::log("3rd Person Camera Mode");
            } else {
                m_CameraMode = eFree;
                ObjectOfType<camera::ICamera>::Provide(std::dynamic_pointer_cast<camera::ICamera>(m_FreeCameraPtr));
                logger::Logger::log("Free Camera Mode");
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

    const CameraManager::CameraMode &CameraManager::Mode() const {
        return m_CameraMode;
    }

    void CameraManager::UpdateFreeCamera() {
        m_FreeCameraPtr->m_PrevPosition = m_FreeCameraPtr->m_Position;
        m_FreeCameraPtr->m_PrevForward = m_FreeCameraPtr->m_Forward;

        float movAmt = m_FreeCameraPtr->m_Speed;
        float rotAmt = m_FreeCameraPtr->m_Speed * m_FreeCameraPtr->m_RotationSensitivity;

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_W))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Forward, movAmt);

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_S))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Forward, -movAmt);

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_A))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Right, -movAmt);

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_D))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Right, movAmt);

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_T))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Up, movAmt);

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_G))
            m_FreeCameraPtr->Move(m_FreeCameraPtr->m_Up, -movAmt);

        if (m_EventHandlerPtr->ButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (m_EventHandlerPtr->MouseMoved()) {
                float yawChange = (float) m_EventHandlerPtr->MouseDX() * rotAmt;
                m_FreeCameraPtr->Rotate(0.0f, -yawChange);
            }
        }

        if (m_EventHandlerPtr->ButtonHolding(GLFW_MOUSE_BUTTON_LEFT)) {
            if (m_EventHandlerPtr->MouseMoved()) {
                float pitchChange = (float) m_EventHandlerPtr->MouseDY() * rotAmt;
                m_FreeCameraPtr->Rotate(-pitchChange, 0.0f);
            }
        }

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_K)) {
            Logger::log("Position (" +
                std::to_string(m_FreeCameraPtr->m_Position.x) + ", " +
                std::to_string(m_FreeCameraPtr->m_Position.y) + ", " +
                std::to_string(m_FreeCameraPtr->m_Position.z) + ")"
            );
        }
    }

    void CameraManager::UpdateFirstPersonCamera() {

    }

    void CameraManager::UpdateThirdPersonCamera() {
        m_ThirdPersonCameraPtr->m_PrevPosition = m_ThirdPersonCameraPtr->m_Position;
        m_ThirdPersonCameraPtr->m_PrevForward = m_ThirdPersonCameraPtr->m_Forward;

        if (m_EventHandlerPtr->MouseScrolled()) {
            auto zoomLevel = (float) m_EventHandlerPtr->MouseDWheel();
            float distanceFromPlayer = m_ThirdPersonCameraPtr->DistanceFromPlayer() - zoomLevel;
            distanceFromPlayer = glm::clamp(distanceFromPlayer, 2.5f, 35.0f);
             m_ThirdPersonCameraPtr->DistanceFromPlayer() = distanceFromPlayer;
        }

        if (m_EventHandlerPtr->ButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE)) {
            if (m_EventHandlerPtr->MouseMoved()) {
                auto pitchChange = (float) m_EventHandlerPtr->MouseDY();
                float pitch = m_ThirdPersonCameraPtr->Pitch() - pitchChange;
                pitch = glm::clamp(pitch, 5.0f, 75.0f);
                m_ThirdPersonCameraPtr->Pitch() = pitch;
            }
        }

        if (m_EventHandlerPtr->ButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE)) {
            if (m_EventHandlerPtr->MouseMoved()) {
                auto angleChange = (float) m_EventHandlerPtr->MouseDX();
                m_ThirdPersonCameraPtr->AngleAroundPlayer() -= angleChange * 2.5f;
            }
        }

        m_ThirdPersonCameraPtr->Update();
    }
}