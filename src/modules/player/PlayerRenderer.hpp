//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PLAYERRENDERER_HPP
#define VD_GAME_ENGINE_PLAYERRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>
#include <modules/shadow/ShadowShader.hpp>

#include <string>

#include "Player.hpp"

namespace mod::player {
    class PlayerRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        PlayerRenderer(PlayerPtr playerPtr,
                       vd::gl::EntityShaderPtr shaderPtr,
                       vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                       vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~PlayerRenderer();

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        PlayerPtr m_pPlayer;

        mod::shadow::ShadowShaderPtr m_pShadowShader;
    };
    typedef std::shared_ptr<PlayerRenderer>	PlayerRendererPtr;
}

#endif //VD_GAME_ENGINE_PLAYERRENDERER_HPP