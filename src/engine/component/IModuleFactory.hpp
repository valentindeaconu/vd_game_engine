//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_IMODULEFACTORY_HPP
#define VD_GAME_ENGINE_IMODULEFACTORY_HPP

#include <engine/kernel/Engine.hpp>

namespace vd::component {
    class IModuleFactory {
    public:
        virtual void Create(const EnginePtr& pEngine) = 0;
    };
}


#endif //VD_GAME_ENGINE_IMODULEFACTORY_HPP
