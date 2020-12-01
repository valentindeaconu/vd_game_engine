//
// Created by Vali on 11/19/2020.
//

#include "PlayerFactory.hpp"

namespace mod::player {

    void PlayerFactory::Create(const vd::EnginePtr& pEngine) {
        PlayerPtr pPlayer = vd::injector::CreateAndStore<Player>();
        PlayerShaderPtr pPlayerShader = std::make_shared<PlayerShader>();

        PlayerRendererPtr pPlayerRenderer = std::make_shared<mod::player::PlayerRenderer>(pPlayer,
                                                                                          pPlayerShader,
                                                                                          []() { glFrontFace(GL_CCW); },
                                                                                          []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(pPlayerRenderer, PlayerRenderer::kDefaultPriority);
    }

}