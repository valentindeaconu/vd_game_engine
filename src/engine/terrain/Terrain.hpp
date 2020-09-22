//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <memory>

#include "TerrainConfig.hpp"
#include "TerrainQuadtree.hpp"

namespace vd::terrain {
    class Terrain : public object::Entity {
    public:
        Terrain(const EnginePtr& enginePtr, const std::string& configFilePath);
        ~Terrain();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const TerrainConfigPtr& getTerrainConfig() const;
        [[nodiscard]] const TerrainQuadtreePtr& getTerrainQuadtree() const;
    private:
        TerrainConfigPtr    configPtr;
        TerrainQuadtreePtr  terrainQuadtreePtr;
    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
