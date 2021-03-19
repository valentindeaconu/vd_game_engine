//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINFACTORY_HPP
#define VD_GAME_ENGINE_TERRAINFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/defines/Types.hpp>

#include "Terrain.hpp"
#include "TerrainRenderer.hpp"
#include "TerrainShader.hpp"

namespace mod::terrain {
    class TerrainFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<TerrainFactory> TerrainFactoryPtr;
}


#endif //VD_GAME_ENGINE_TERRAINFACTORY_HPP
