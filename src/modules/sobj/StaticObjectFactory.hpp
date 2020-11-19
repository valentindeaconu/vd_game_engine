//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_STATICOBJECTFACTORY_HPP
#define VD_GAME_ENGINE_STATICOBJECTFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/misc/Types.hpp>

#include "StaticObjectRenderer.hpp"
#include "StaticObjectPlacer.hpp"
#include "StaticObjectShader.hpp"

namespace mod::sobj {
    class StaticObjectFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<StaticObjectFactory> StaticObjectFactoryPtr;
}


#endif //VD_GAME_ENGINE_STATICOBJECTFACTORY_HPP
