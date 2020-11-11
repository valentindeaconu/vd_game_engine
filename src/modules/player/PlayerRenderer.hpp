#ifndef __PLAYER_RENDERER_HPP_
#define __PLAYER_RENDERER_HPP_

#include <engine/component/Renderer.hpp>

#include <string>

#include "Player.hpp"

namespace mod::player {
    class PlayerRenderer : public vd::component::Renderer {
    public:
        PlayerRenderer();
        ~PlayerRenderer();

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        PlayerPtr& GetPlayer();
        [[nodiscard]] const PlayerPtr& GetPlayer() const;
        void SetPlayer(const PlayerPtr& playerPtr);

    private:
        bool IsReady() override;

        PlayerPtr m_PlayerPtr;
    };
    typedef std::shared_ptr<PlayerRenderer>	PlayerRendererPtr;
}

#endif // !__PLAYER_RENDERER_HPP_