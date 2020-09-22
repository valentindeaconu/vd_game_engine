//
// Created by Vali on 9/21/2020.
//

#include "TerrainQuadtree.hpp"

namespace vd::terrain {

    TerrainQuadtree::TerrainQuadtree(const EnginePtr& enginePtr, const TerrainConfigPtr& terrainConfigPtr)
        : object::Entity(enginePtr)
        , configPtr(terrainConfigPtr)
    {
    }

    TerrainQuadtree::~TerrainQuadtree() = default;

    void TerrainQuadtree::init() {
        int rootNodes = configPtr->getRootNodes();

        nodes.reserve(rootNodes * rootNodes);

        for (int i = 0; i < rootNodes; ++i) {
            for (int j = 0; j < rootNodes; ++j) {
                glm::vec2 nodePosition(float(i) / float(rootNodes), float(j) / float(rootNodes));

                TerrainNodePtr nodePtr =
                        std::make_shared<TerrainNode>(getParentEngine(),
                                                      configPtr,
                                                      nodePosition,
                                                      0,
                                                      glm::vec2(i, j));

                nodePtr->init();

                nodes.push_back(std::move(nodePtr));
            }
        }

        getWorldTransform().setScaling(configPtr->getScaleXZ(), configPtr->getScaleY(), configPtr->getScaleXZ());

        generatePatch();

        Entity::init();
    }

    void TerrainQuadtree::update() {
        for (auto& nodePtr : nodes) {
            nodePtr->update();
        }
    }

    void TerrainQuadtree::cleanUp() {
        Entity::cleanUp();
    }

    const TerrainNodePtrVec& TerrainQuadtree::getRootNodes() const {
        return nodes;
    }

    void TerrainQuadtree::generatePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        auto& vertices = meshPtr->vertices;
        vertices.resize(16);

        vertices[0] = { .Position = glm::vec3(0.0f, 0.0f, 0.0f) };
        vertices[1] = { .Position = glm::vec3(0.333f, 0.0f, 0.0f) };
        vertices[2] = { .Position = glm::vec3(0.666f, 0.0f, 0.0f) };
        vertices[3] = { .Position = glm::vec3(1.0f, 0.0f, 0.0f) };

        vertices[4] = { .Position = glm::vec3(0.0f, 0.0f, 0.333f) };
        vertices[5] = { .Position = glm::vec3(0.333f, 0.0f, 0.333f) };
        vertices[6] = { .Position = glm::vec3(0.666f, 0.0f, 0.333f) };
        vertices[7] = { .Position = glm::vec3(1.0f, 0.0f, 0.333f) };

        vertices[8] = { .Position = glm::vec3(0.0f, 0.0f, 0.666f) };
        vertices[9] = { .Position = glm::vec3(0.333f, 0.0f, 0.666f) };
        vertices[10] = { .Position = glm::vec3(0.666f, 0.0f, 0.666f) };
        vertices[11] = { .Position = glm::vec3(1.0f, 0.0f, 0.666f) };

        vertices[12] = { .Position = glm::vec3(0.0f, 0.0f, 1.0f) };
        vertices[13] = { .Position = glm::vec3(0.333f, 0.0f, 1.0f) };
        vertices[14] = { .Position = glm::vec3(0.666f, 0.0f, 1.0f) };
        vertices[15] = { .Position = glm::vec3(1.0f, 0.0f, 1.0f) };

        setBufferGenerationStrategy(ePatch);

        getMeshes().push_back(meshPtr);
    }

}