//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_SKYFACTORY_HPP
#define VD_GAME_ENGINE_SKYFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/defines/Types.hpp>

#include "Sky.hpp"
#include "SkyShader.hpp"
#include "SkyRenderer.hpp"

#include "Sun.hpp"
#include "SunShader.hpp"
#include "SunRenderer.hpp"

#include "LensFlareRenderer.hpp"

namespace mod::sky {
    class SkyFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<SkyFactory> SkyFactoryPtr;
}


#endif //VD_GAME_ENGINE_SKYFACTORY_HPP
