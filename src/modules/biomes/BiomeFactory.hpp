//
// Created by Vali on 3/26/2021.
//

#ifndef VDGE_BIOMEFACTORY_HPP
#define VDGE_BIOMEFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>

#include "BiomeManager.hpp"

namespace mod::biomes {

    class BiomeFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<BiomeFactory> BiomeFactoryPtr;

}

#endif //VDGE_BIOMEFACTORY_HPP