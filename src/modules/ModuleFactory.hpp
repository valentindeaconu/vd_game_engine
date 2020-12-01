//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_MODULEFACTORY_HPP
#define VD_GAME_ENGINE_MODULEFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

namespace mod {
    class ModuleFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<ModuleFactory>  ModuleFactoryPtr;
}


#endif //VD_GAME_ENGINE_MODULEFACTORY_HPP
