#ifndef __PLAYER_HPP_
#define __PLAYER_HPP_

#include <engine/object/Entity.hpp>

#include <engine/foundation/objloader/OBJLoader.hpp>

#include <engine/core/ObjectOfType.hpp>

#include <engine/kernel/EngineBlock.hpp>
#include <engine/core/InputHandler.hpp>
#include <modules/terrain/Terrain.hpp>

namespace mod::player {
    class Player : public vd::object::Entity {
    public:
        Player();
        ~Player();

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        void Jump();
        void Input();

        const float m_kModelYOffset;

        float m_CurrentSpeed;
        float m_CurrentTurnSpeed;
        float m_CurrentUpwardsSpeed;
        bool m_IsJumping;

        vd::EnginePtr m_EnginePtr;
        vd::core::InputHandlerPtr m_InputHandlerPtr;
        mod::terrain::TerrainPtr m_TerrainPtr;

        const float m_kRunSpeed = 20.0f; // units per second
        const float m_kTurnSpeed = 160.f; // degrees per second
        const float m_kGravity = -50.f; // units per second
        const float m_kJumpPower = 25.0f; // units per second
    };
    typedef std::shared_ptr<Player>	PlayerPtr;
}

#endif // !__PLAYER_HPP_
