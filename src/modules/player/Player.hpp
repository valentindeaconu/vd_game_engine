//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PLAYER_HPP
#define VD_GAME_ENGINE_PLAYER_HPP

#include <engine/object/Entity3D.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/kernel/Context.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/event/EventHandler.hpp>
#include <engine/loader/ObjectLoader.hpp>
#include <modules/terrain/Terrain.hpp>

namespace mod::player {
    class Player : public vd::object::Entity3D, public vd::injector::Injectable {
    public:
        Player();

        void Setup() override;

        void Link() override;

        void Init() override;
        void Update() override;

        [[nodiscard]] float ModelYOffset() const;
    private:
        void Jump();
        void Input();

        bool m_Jumping;

        float m_CurrentSpeed;
        float m_CurrentTurnSpeed;
        float m_CurrentUpwardsSpeed;

        const float m_kModelYOffset;

        vd::kernel::ContextPtr          m_pContext;
        vd::camera::CameraManagerPtr    m_pCameraManager;
        vd::event::EventHandlerPtr      m_pEventHandler;
        mod::terrain::TerrainPtr        m_pTerrain;

        const float m_kRunSpeed = 20.0f; // units per second
        const float m_kTurnSpeed = 160.f; // degrees per second
        const float m_kGravity = -50.f; // units per second
        const float m_kJumpPower = 25.0f; // units per second
    };
    typedef std::shared_ptr<Player>	PlayerPtr;
}

#endif //VD_GAME_ENGINE_PLAYER_HPP
