#ifndef __PLAYER_HPP_
#define __PLAYER_HPP_

#include <engine/object/Entity.hpp>
#include <engine/foundation/objloader/OBJLoader.hpp>

#include <modules/terrain/Terrain.hpp>

namespace mod::player
{
    class Player : public vd::object::Entity
    {
    public:
        Player(const vd::EnginePtr& enginePtr, const mod::terrain::TerrainPtr& terrainPtr);
        ~Player();

        void init() override;
        void update() override;
        void cleanUp() override;
    private:
        void jump();
        void input();

        float currentSpeed;
        float currentTurnSpeed;
        float currentUpwardsSpeed;
        bool isJumping;

        mod::terrain::TerrainPtr terrainPtr;

        const float modelYOffset = 1.0f;

        const float kRunSpeed = 20.0f; // units per second
        const float kTurnSpeed = 160.f; // degrees per second
        const float kGravity = -50.f; // units per second
        const float kJumpPower = 25.0f; // units per second
    };
    typedef std::shared_ptr<Player>	PlayerPtr;
}

#endif // !__PLAYER_HPP_
