//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PLAYERRENDERER_HPP
#define VD_GAME_ENGINE_PLAYERRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <modules/shadow/ShadowShader.hpp>

#include <engine/collision/Detector.hpp>

#include <string>

#include "Player.hpp"

namespace mod::player {
    class PlayerRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        PlayerRenderer(PlayerPtr player, vd::component::IEntityShaderPtr shader);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        PlayerPtr                       m_pPlayer;
        vd::component::IEntityShaderPtr m_pShader;

        vd::camera::CameraPtr           m_pCamera;
        mod::shadow::ShadowShaderPtr    m_pShadowShader;
    };
    typedef std::shared_ptr<PlayerRenderer>	PlayerRendererPtr;
}

#endif //VD_GAME_ENGINE_PLAYERRENDERER_HPP