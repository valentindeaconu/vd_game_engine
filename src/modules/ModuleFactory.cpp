//
// Created by Vali on 11/19/2020.
//

#include "ModuleFactory.hpp"

// Factories
#include <modules/gui/GuiFactory.hpp>
#include <modules/player/PlayerFactory.hpp>
#include <modules/postprocessing/EffectFactory.hpp>
#include <modules/shadow/ShadowFactory.hpp>
#include <modules/sky/SkyFactory.hpp>
#include <modules/props/PropsFactory.hpp>
#include <modules/terrain/TerrainFactory.hpp>
#include <modules/water/WaterFactory.hpp>
#include <modules/biomes/BiomeFactory.hpp>
#include <modules/particles/ParticleFactory.hpp>

namespace mod {

    void ModuleFactory::Create(const vd::EnginePtr& pEngine) {
        std::make_shared<shadow::ShadowFactory>()->Create(pEngine);

        std::make_shared<biomes::BiomeFactory>()->Create(pEngine);
        std::make_shared<terrain::TerrainFactory>()->Create(pEngine);
        std::make_shared<player::PlayerFactory>()->Create(pEngine);
        std::make_shared<water::WaterFactory>()->Create(pEngine);
        std::make_shared<sky::SkyFactory>()->Create(pEngine);
        std::make_shared<props::PropsFactory>()->Create(pEngine);
        std::make_shared<particles::ParticleFactory>()->Create(pEngine);

        std::make_shared<gui::GuiFactory>()->Create(pEngine);
        std::make_shared<postprocessing::EffectFactory>()->Create(pEngine);
    }

}
