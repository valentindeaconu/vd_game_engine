//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINQUADTREE_HPP
#define VD_GAME_ENGINE_TERRAINQUADTREE_HPP

#include <engine/object/Entity.hpp>

#include <memory>

#include "TerrainConfig.hpp"
#include "TerrainNode.hpp"

namespace vd::terrain {
    class TerrainQuadtree : public object::Entity {
    public:
        static constexpr int kRootNodes = 8;

        TerrainQuadtree(const EnginePtr& enginePtr, const TerrainConfigPtr& terrainConfigPtr);
        ~TerrainQuadtree();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const TerrainNodePtrVec& getRootNodes() const;
    private:
        void generatePatch();

        TerrainConfigPtr configPtr;

        TerrainNodePtrVec nodes;
    };
    typedef std::shared_ptr<TerrainQuadtree>    TerrainQuadtreePtr;
}


#endif //VD_GAME_ENGINE_TERRAINQUADTREE_HPP
