#include "Player.hpp"

namespace mod::player {
    Player::Player()
        : m_kModelYOffset(1.0f)
        , m_CurrentSpeed(0.0f)
        , m_CurrentTurnSpeed(0.0f)
        , m_CurrentUpwardsSpeed(0.0f)
        , m_Jumping(false)
    {
    }

    void Player::Setup() {
        this->PushMesh(vd::loader::ObjectLoader::Load("./resources/assets/nanosuit/nanosuit.obj"), 1000.0f);
    }

    void Player::Link() {
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
        m_pCameraManager = vd::ObjectOfType<vd::camera::CameraManager>::Find();
        m_pTerrain = vd::ObjectOfType<mod::terrain::Terrain>::Find();
        m_pEventHandler = vd::ObjectOfType<vd::event::EventHandler>::Find();
    }

    void Player::Init() {
        Entity3D::Init();

        float h = m_pTerrain->HeightAt(0.0f, 0.0f);
        WorldTransform().Translation() = glm::vec3(0.0f, h + m_kModelYOffset, 0.0f);
    }

    void Player::Update() {
        if (m_pCameraManager->Mode() == vd::camera::CameraManager::eThirdPerson)
            Input();

        float currentAngle = WorldTransform().YAxisRotationAngle();
        currentAngle += m_CurrentTurnSpeed * m_pContext->FrameTime();

        if (currentAngle >= 360.0f) {
            currentAngle -= 360.0f;
        } else if (currentAngle < 0.0f) {
            currentAngle = 360.0f - currentAngle;
        }

        WorldTransform().YAxisRotationAngle() = currentAngle;

        float distance = m_CurrentSpeed * m_pContext->FrameTime();
        float dx = distance * glm::sin(glm::radians(currentAngle));
        float dz = distance * glm::cos(glm::radians(currentAngle));

        glm::vec3 currentPosition = WorldTransform().Translation();
        currentPosition.x += dx;
        currentPosition.z += dz;

        m_CurrentUpwardsSpeed += m_kGravity * m_pContext->FrameTime();
        currentPosition.y += m_CurrentUpwardsSpeed * m_pContext->FrameTime();

        float height = m_pTerrain->HeightAt(currentPosition.x, currentPosition.z);

        if (currentPosition.y < height + m_kModelYOffset) {
            m_CurrentUpwardsSpeed = 0.0f;
            currentPosition.y = height + m_kModelYOffset;
            m_Jumping = false;
        }

        WorldTransform().Translation() = currentPosition;
    }

    float Player::ModelYOffset() const {
        return m_kModelYOffset;
    }

    void Player::Jump() {
        if (!m_Jumping)
        {
            m_CurrentUpwardsSpeed = m_kJumpPower;
            m_Jumping = true;
        }
    }

    void Player::Input() {
        if (m_pEventHandler->KeyHolding(GLFW_KEY_W)) {
            this->m_CurrentSpeed = m_kRunSpeed;
        } else if (m_pEventHandler->KeyHolding(GLFW_KEY_S)) {
            this->m_CurrentSpeed = -m_kRunSpeed;
        } else {
            this->m_CurrentSpeed = 0.0f;
        }

        if (m_pEventHandler->KeyHolding(GLFW_KEY_D)) {
            this->m_CurrentTurnSpeed = -m_kTurnSpeed;
        } else if (m_pEventHandler->KeyHolding(GLFW_KEY_A)) {
            this->m_CurrentTurnSpeed = m_kTurnSpeed;
        } else {
            this->m_CurrentTurnSpeed = 0.0f;
        }

        if (m_pEventHandler->KeyDown(GLFW_KEY_SPACE)) {
            Jump();
        }
    }
}