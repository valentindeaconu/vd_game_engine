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
        m_TerrainPtr = vd::ObjectOfType<mod::terrain::Terrain>::Find();
        m_InputHandlerPtr = vd::ObjectOfType<vd::core::InputHandler>::Find();

        float h = m_TerrainPtr->GetHeight(0.0f, 0.0f);
        GetWorldTransform().setTranslation(0.0f, h + m_kModelYOffset, 0.0f);

        vd::model::MeshPtrVec& meshPtrVec = GetMeshes();
        vd::objloader::OBJLoader objLoader;
        objLoader.load("./resources/objects/nanosuit", "nanosuit.obj", meshPtrVec);

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void Player::Update() {
        if (m_EnginePtr->getCameraMode() == vd::Engine::e3rdPersonCamera)
            Input();

        float currentAngle = GetWorldTransform().getYAxisRotationAngle();
        currentAngle += m_CurrentTurnSpeed * m_EnginePtr->getFrameTime();

        if (currentAngle >= 360.0f) {
            currentAngle -= 360.0f;
        } else if (currentAngle < 0.0f) {
            currentAngle = 360.0f - currentAngle;
        }

        GetWorldTransform().setYRotationAngle(currentAngle);

        float distance = m_CurrentSpeed * m_EnginePtr->getFrameTime();
        float dx = distance * glm::sin(glm::radians(currentAngle));
        float dz = distance * glm::cos(glm::radians(currentAngle));

        glm::vec3 currentPosition = GetWorldTransform().getTranslationVector();
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

        GetWorldTransform().setTranslation(currentPosition);
    }

    void Player::CleanUp() {
        Entity::CleanUp(); // call super.CleanUp() to clear meshBuffers;
    }

    void Player::Jump() {
        if (!m_IsJumping)
        {
            m_CurrentUpwardsSpeed = m_kJumpPower;
            m_IsJumping = true;
        }
    }

    void Player::Input() {
        if (m_InputHandlerPtr->getKeyHolding(GLFW_KEY_W)) {
            this->m_CurrentSpeed = m_kRunSpeed;
        } else if (m_InputHandlerPtr->getKeyHolding(GLFW_KEY_S)) {
            this->m_CurrentSpeed = -m_kRunSpeed;
        } else {
            this->m_CurrentSpeed = 0.0f;
        }

        if (m_InputHandlerPtr->getKeyHolding(GLFW_KEY_D)) {
            this->m_CurrentTurnSpeed = -m_kTurnSpeed;
        } else if (m_InputHandlerPtr->getKeyHolding(GLFW_KEY_A)) {
            this->m_CurrentTurnSpeed = m_kTurnSpeed;
        } else {
            this->m_CurrentTurnSpeed = 0.0f;
        }

        if (m_InputHandlerPtr->getKeyDown(GLFW_KEY_SPACE)) {
            Jump();
        }
    }
}