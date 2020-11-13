#include "Player.hpp"

namespace mod::player {
    Player::Player()
        : m_kModelYOffset(1.0f)
        , m_CurrentSpeed(0.0f)
        , m_CurrentTurnSpeed(0.0f)
        , m_CurrentUpwardsSpeed(0.0f)
        , m_IsJumping(false)
    {
    }

    Player::~Player() = default;

    void Player::Init() {
        m_EnginePtr = vd::ObjectOfType<vd::Engine>::Find();
        m_CameraManagerPtr = vd::ObjectOfType<vd::camera::CameraManager>::Find();
        m_TerrainPtr = vd::ObjectOfType<mod::terrain::Terrain>::Find();
        m_EventHandlerPtr = vd::ObjectOfType<vd::event::EventHandler>::Find();

        float h = m_TerrainPtr->GetHeight(0.0f, 0.0f);
        WorldTransform().SetTranslation(0.0f, h + m_kModelYOffset, 0.0f);

        this->Meshes() = vd::loader::ObjectLoader::Load("./resources/objects/nanosuit/nanosuit.obj");

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void Player::Update() {
        if (m_CameraManagerPtr->Mode() == vd::camera::CameraManager::eThirdPerson)
            Input();

        float currentAngle = WorldTransform().GetYAxisRotationAngle();
        currentAngle += m_CurrentTurnSpeed * m_EnginePtr->getFrameTime();

        if (currentAngle >= 360.0f) {
            currentAngle -= 360.0f;
        } else if (currentAngle < 0.0f) {
            currentAngle = 360.0f - currentAngle;
        }

        WorldTransform().SetYRotationAngle(currentAngle);

        float distance = m_CurrentSpeed * m_EnginePtr->getFrameTime();
        float dx = distance * glm::sin(glm::radians(currentAngle));
        float dz = distance * glm::cos(glm::radians(currentAngle));

        glm::vec3 currentPosition = WorldTransform().GetTranslationVector();
        currentPosition.x += dx;
        currentPosition.z += dz;

        m_CurrentUpwardsSpeed += m_kGravity * m_EnginePtr->getFrameTime();
        currentPosition.y += m_CurrentUpwardsSpeed * m_EnginePtr->getFrameTime();

        float height = m_TerrainPtr->GetHeight(currentPosition.x, currentPosition.z);

        if (currentPosition.y < height + m_kModelYOffset) {
            m_CurrentUpwardsSpeed = 0.0f;
            currentPosition.y = height + m_kModelYOffset;
            m_IsJumping = false;
        }

        WorldTransform().SetTranslation(currentPosition);
    }

    void Player::CleanUp() {
        Entity::CleanUp(); // call super.CleanUp() to clear meshBuffers;
    }

    float Player::ModelYOffset() const {
        return m_kModelYOffset;
    }

    void Player::Jump() {
        if (!m_IsJumping)
        {
            m_CurrentUpwardsSpeed = m_kJumpPower;
            m_IsJumping = true;
        }
    }

    void Player::Input() {
        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_W)) {
            this->m_CurrentSpeed = m_kRunSpeed;
        } else if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_S)) {
            this->m_CurrentSpeed = -m_kRunSpeed;
        } else {
            this->m_CurrentSpeed = 0.0f;
        }

        if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_D)) {
            this->m_CurrentTurnSpeed = -m_kTurnSpeed;
        } else if (m_EventHandlerPtr->KeyHolding(GLFW_KEY_A)) {
            this->m_CurrentTurnSpeed = m_kTurnSpeed;
        } else {
            this->m_CurrentTurnSpeed = 0.0f;
        }

        if (m_EventHandlerPtr->KeyDown(GLFW_KEY_SPACE)) {
            Jump();
        }
    }
}