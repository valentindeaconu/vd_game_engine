//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINNODE_HPP
#define VD_GAME_ENGINE_TERRAINNODE_HPP

#include <glm/glm.hpp>

#include <engine/core/Camera.hpp>

#include <engine/object/Entity.hpp>

#include <memory>

#include "TerrainConfig.hpp"

namespace vd::terrain {
    class TerrainNode;
    typedef std::shared_ptr<TerrainNode>    TerrainNodePtr;
    typedef std::vector<TerrainNodePtr>     TerrainNodePtrVec;

    class TerrainNode : public object::Entity {
    public:
        TerrainNode(const EnginePtr& enginePtr,
                    const TerrainConfigPtr& terrainConfigPtr,
                    const glm::vec2& position,
                    int lod,
                    const glm::vec2& index);
        ~TerrainNode();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const TerrainConfigPtr& getConfig() const;

        [[nodiscard]] bool isLeaf() const;
        [[nodiscard]] int getLod() const;
        [[nodiscard]] float getGap() const;

        [[nodiscard]] const glm::vec2& getPosition() const;
        [[nodiscard]] const glm::vec3& getWorldPosition() const;
        [[nodiscard]] const glm::vec2& getIndex() const;

        [[nodiscard]] const TerrainNodePtrVec& getNodes() const;
    private:
        void computeWorldPosition();

        void addNodes(int lod);
        void updateNodes();
        void clearNodes();

        TerrainConfigPtr configPtr;

        bool leaf;
        int lod;
        float gap;
        glm::vec2 position;
        glm::vec3 worldPosition;
        glm::vec2 index;

        TerrainNodePtrVec nodes;
    };
}


#endif //VD_GAME_ENGINE_TERRAINNODE_HPP
