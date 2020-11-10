#include "Player.hpp"

namespace mod::player
{
    Player::Player(const vd::EnginePtr& enginePtr, const mod::terrain::TerrainPtr& terrainPtr)
        : Entity(enginePtr)
        , terrainPtr(terrainPtr)
    {
    }

    Player::~Player() = default;

    void Player::init()
    {
        //getWorldTransform().setTranslation(256.0f, 0.0f, 256.0f);
        //getWorldTransform().setTranslation(0.0f, 0.0f, 0.0f);

        float h = terrainPtr->GetTerrainConfig()->getHeight(0.0f, 0.0f);

        getWorldTransform().setTranslation(0.0f, h + modelYOffset, 0.0f);
        //getWorldTransform().setScaling(0.5f, 0.5f, 0.5f);
        vd::objloader::OBJLoaderPtr objLoaderPtr = std::make_shared<vd::objloader::OBJLoader>();

        vd::model::MeshPtrVec& meshPtrVec = getMeshes();
        objLoaderPtr->load("./resources/objects/myriam", "myriam.obj", meshPtrVec);

        Entity::init(); // call super.init() to initialize meshBuffers;
    }

    void Player::update() {
        if (this->getParentEngine()->getCameraMode() == vd::Engine::e3rdPersonCamera)
            input();

        float currentAngle = getWorldTransform().getYAxisRotationAngle();
        currentAngle += currentTurnSpeed * getParentEngine()->getFrameTime();

        if (currentAngle >= 360.0f)
        {
            currentAngle -= 360.0f;
        }
        else if (currentAngle < 0.0f)
        {
            currentAngle = 360.0f - currentAngle;
        }

        getWorldTransform().setYRotationAngle(currentAngle);

        float distance = currentSpeed * getParentEngine()->getFrameTime();
        float dx = distance * glm::sin(glm::radians(currentAngle));
        float dz = distance * glm::cos(glm::radians(currentAngle));

        glm::vec3 currentPosition = getWorldTransform().getTranslationVector();
        currentPosition.x += dx;
        currentPosition.z += dz;

        currentUpwardsSpeed += kGravity * getParentEngine()->getFrameTime();
        currentPosition.y += currentUpwardsSpeed * getParentEngine()->getFrameTime();

        float height = terrainPtr->GetTerrainConfig()->getHeight(currentPosition.x, currentPosition.z);

        if (currentPosition.y < height + modelYOffset) {
            currentUpwardsSpeed = 0.0f;
            currentPosition.y = height + modelYOffset;
            isJumping = false;
        }

        getWorldTransform().setTranslation(currentPosition);
    }

    void Player::cleanUp()
    {
        Entity::cleanUp(); // call super.cleanUp() to clear meshBuffers;
    }

    void Player::jump()
    {
        if (!isJumping)
        {
            currentUpwardsSpeed = kJumpPower;
            isJumping = true;
        }
    }

    void Player::input()
    {
        auto& handler = getParentEngine()->getInputHandler();

        if (handler->getKeyHolding(GLFW_KEY_W))
        {
            this->currentSpeed = kRunSpeed;
        }
        else if (handler->getKeyHolding(GLFW_KEY_S))
        {
            this->currentSpeed = -kRunSpeed;
        }
        else
        {
            this->currentSpeed = 0.0f;
        }

        if (handler->getKeyHolding(GLFW_KEY_D))
        {
            this->currentTurnSpeed = -kTurnSpeed;
        }
        else if (handler->getKeyHolding(GLFW_KEY_A))
        {
            this->currentTurnSpeed = kTurnSpeed;
        }
        else
        {
            this->currentTurnSpeed = 0.0f;
        }

        if (handler->getKeyDown(GLFW_KEY_SPACE))
        {
            jump();
        }
    }
}