//
// Created by Vali on 3/26/2021.
//

#include "BiomeFactory.hpp"


namespace mod::biomes {

    void BiomeFactory::Create(const vd::EnginePtr& pEngine) {
        BiomeManagerPtr pBiomeManager = vd::injector::CreateAndStore<BiomeManager>("./resources/properties/biomes.properties");

        pEngine->Subscribe(pBiomeManager, BiomeManager::kPriority + 1);
    }

}