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

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        PlayerPtr& getPlayer();
        [[nodiscard]] const PlayerPtr& getPlayer() const;
        void setPlayer(const PlayerPtr& playerPtr);

    private:
        bool isReady() override;

        PlayerPtr playerPtr;
    };
    typedef std::shared_ptr<PlayerRenderer>	PlayerRendererPtr;
}

#endif // !__PLAYER_RENDERER_HPP_