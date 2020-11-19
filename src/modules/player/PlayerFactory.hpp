//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_PLAYERFACTORY_HPP
#define VD_GAME_ENGINE_PLAYERFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/misc/Types.hpp>

#include "Player.hpp"
#include "PlayerShader.hpp"
#include "PlayerRenderer.hpp"

namespace mod::player {
    class PlayerFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<PlayerFactory> PlayerFactoryPtr;
}


#endif //VD_GAME_ENGINE_PLAYERFACTORY_HPP
