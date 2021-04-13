//
// Created by Vali on 11/19/2020.
//

#include "TerrainFactory.hpp"

namespace mod::terrain {

    void TerrainFactory::Create(const vd::EnginePtr& pEngine) {
        TerrainPtr pTerrain = vd::injector::CreateAndStore<Terrain>("./resources/properties/terrain.properties");
        TerrainShaderPtr pTerrainShader = std::make_shared<TerrainShader>();

        TerrainRendererPtr pTerrainRenderer = std::make_shared<TerrainRenderer>(pTerrain, pTerrainShader);

        pEngine->Subscribe(pTerrainRenderer, TerrainRenderer::kPriority);
    }

}
