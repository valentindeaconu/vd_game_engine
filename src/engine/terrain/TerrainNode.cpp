//
// Created by Vali on 9/21/2020.
//

#include "TerrainNode.hpp"

namespace vd::terrain {

    TerrainNode::TerrainNode(const EnginePtr& enginePtr,
                             const TerrainConfigPtr& terrainConfigPtr,
                             const glm::vec2& position,
                             int lod,
                             const glm::vec2& index)
        : object::Entity(enginePtr)
        , configPtr(terrainConfigPtr)
        , position(position)
        , lod(lod)
        , index(index)
        , leaf(false)
        , worldPosition(0.0f)
        , gap(0.0f)
    {
    }

    TerrainNode::~TerrainNode() = default;

    void TerrainNode::init() {
        gap = 1.0f / (float(configPtr->getRootNodes()) * float(1 << lod));

        getLocalTransform().setScaling(gap, 0.0f, gap);
        getLocalTransform().setTranslation(position.x, 0.0f, position.y);

        computeWorldPosition();

        Entity::init();
    }

    void TerrainNode::update() {
        auto& cameraPtr = getParentEngine()->getCamera();

        updateNodes();

        for (auto& nodePtr : nodes) {
            nodePtr->update();
        }
    }

    void TerrainNode::cleanUp() {
        Entity::cleanUp();
    }

    const TerrainConfigPtr& TerrainNode::getConfig() const {
        return configPtr;
    }

    bool TerrainNode::isLeaf() const {
        return leaf;
    }

    int TerrainNode::getLod() const {
        return lod;
    }

    float TerrainNode::getGap() const {
        return gap;
    }

    const glm::vec2& TerrainNode::getPosition() const {
        return position;
    }

    const glm::vec3& TerrainNode::getWorldPosition() const {
        return worldPosition;
    }

    const glm::vec2& TerrainNode::getIndex() const {
        return index;
    }

    const TerrainNodePtrVec& TerrainNode::getNodes() const {
        return nodes;
    }

    void TerrainNode::computeWorldPosition() {
        glm::vec2 pos = ((position + (gap / 2.0f)) * configPtr->getScaleXZ());
        float h = configPtr->getHeight(pos.x, pos.y);
        worldPosition = glm::vec3(
            pos.x - (configPtr->getScaleXZ() / 2.0f),
            h,
            pos.y - (configPtr->getScaleXZ() / 2.0f)
        );
    }

    void TerrainNode::addNodes(int childLod) {
        if (leaf) {
            leaf = false;
        }

        auto& cameraPtr = getParentEngine()->getCamera();
        if (nodes.empty()) {
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    glm::vec2 nodePosition(float(i) * gap / 2.0f, float(j) * gap / 2.0f);

                    TerrainNodePtr nodePtr =
                            std::make_shared<TerrainNode>(getParentEngine(),
                                                          configPtr,
                                                          position + nodePosition,
                                                          childLod,
                                                          glm::vec2(i, j));

                    nodePtr->init();

                    nodes.push_back(std::move(nodePtr));
                }
            }
        }
    }

    void TerrainNode::updateNodes() {
        auto& cameraPtr = getParentEngine()->getCamera();

        float distance = glm::length(cameraPtr->getPosition() - worldPosition);

        if (distance < configPtr->getLodRange()[lod]) {
            addNodes(lod + 1);
        } else {
            clearNodes();
        }
    }

    void TerrainNode::clearNodes() {
        if (!leaf) {
            leaf = true;
        }

        if (!nodes.empty()) {
            nodes.clear();
        }
    }
}
