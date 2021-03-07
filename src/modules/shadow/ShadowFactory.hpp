//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWFACTORY_HPP
#define VD_GAME_ENGINE_SHADOWFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/misc/Types.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/context/Context.hpp>
#include <engine/time/TimeManager.hpp>

#include "ShadowShader.hpp"
#include "ShadowManager.hpp"

namespace mod::shadow {
    class ShadowFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<ShadowFactory> ShadowFactoryPtr;
}


#endif //VD_GAME_ENGINE_SHADOWFACTORY_HPP
