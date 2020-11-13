#ifndef __PLAYER_RENDERER_HPP_
#define __PLAYER_RENDERER_HPP_

#include <engine/component/IRenderer.hpp>

#include <modules/shadow/ShadowShader.hpp>

#include <string>

#include "Player.hpp"

namespace mod::player {
    class PlayerRenderer : public vd::component::IRenderer {
    public:
        PlayerRenderer(PlayerPtr playerPtr,
                       vd::shader::ShaderPtr shaderPtr,
                       vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                       vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~PlayerRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        PlayerPtr m_PlayerPtr;
    };
    typedef std::shared_ptr<PlayerRenderer>	PlayerRendererPtr;
}

#endif // !__PLAYER_RENDERER_HPP_