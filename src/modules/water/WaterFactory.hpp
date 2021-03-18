//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_WATERFACTORY_HPP
#define VD_GAME_ENGINE_WATERFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/defines/Types.hpp>

#include "Water.hpp"
#include "WaterRenderer.hpp"
#include "WaterShader.hpp"

namespace mod::water {
    class WaterFactory: public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<WaterFactory> WaterFactoryPtr;
}


#endif //VD_GAME_ENGINE_WATERFACTORY_HPP
