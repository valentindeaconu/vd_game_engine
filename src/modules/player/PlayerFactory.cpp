//
// Created by Vali on 11/19/2020.
//

#include "PlayerFactory.hpp"

namespace mod::player {

    void PlayerFactory::Create(const vd::EnginePtr& pEngine) {
        PlayerPtr pPlayer = vd::injector::CreateAndStore<Player>();
        PlayerShaderPtr pPlayerShader = std::make_shared<mod::player::PlayerShader>();

        PlayerRendererPtr pPlayerRenderer = std::make_shared<mod::player::PlayerRenderer>(pPlayer, pPlayerShader);

        pEngine->Subscribe(pPlayerRenderer, PlayerRenderer::kDefaultPriority);
    }

}