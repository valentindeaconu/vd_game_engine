//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_PROPSFACTORY_HPP
#define VD_GAME_ENGINE_PROPSFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/misc/Types.hpp>

#include "PropsRenderer.hpp"
#include "PropShader.hpp"

namespace mod::props {
    class PropsFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<PropsFactory> PropsFactoryPtr;
}


#endif //VD_GAME_ENGINE_PROPSFACTORY_HPP
