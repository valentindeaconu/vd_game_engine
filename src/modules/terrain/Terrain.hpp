//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <memory>
#include <vector>

#include "TerrainConfig.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class Terrain : public vd::object::Entity {
    public:
        Terrain(const vd::EnginePtr& enginePtr, const std::string& configFilePath);
        ~Terrain();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const TerrainConfigPtr& GetTerrainConfig() const;

        [[nodiscard]] const TerrainNodePtrVec& GetRootNodes() const;

    private:
        void generatePatch();

        TerrainConfigPtr m_ConfigPtr;

        TerrainNodePtrVec m_RootNodes;
    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
