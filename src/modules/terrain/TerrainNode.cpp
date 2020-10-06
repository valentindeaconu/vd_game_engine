//
// Created by Vali on 10/6/2020.
//

#include "TerrainNode.hpp"

namespace mod::terrain {
    TerrainNode::TerrainNode(const TerrainConfigPtr& terrainConfigPtr,
                             const glm::vec2& topLeft,
                             const glm::vec2& bottomRight,
                             int level)
            : m_kConfigPtr(terrainConfigPtr)
            , m_kTopLeft(topLeft)
            , m_kBottomRight(bottomRight)
            , m_kLevel(level)
            , m_Leaf(true)
            , m_kCenter((topLeft.x + bottomRight.x) / 2.0f, (topLeft.y + bottomRight.y) / 2.0f)
    {
        m_Transform.setTranslation(topLeft.x, 0.0f, topLeft.y);
        m_Transform.setScaling(bottomRight.x - topLeft.x, 1.0f, bottomRight.y - topLeft.y);

        m_PatchHeights[0] = computeHeightFor2DPoint(0.0f, 0.0f);
        // m_PatchHeights[1] = computeHeightFor2DPoint(0.333f, 0.0f);
        // m_PatchHeights[2] = computeHeightFor2DPoint(0.666f, 0.0f);
        m_PatchHeights[3] = computeHeightFor2DPoint(1.0f, 0.0f);

        // m_PatchHeights[4] = computeHeightFor2DPoint(0.0f, 0.333f);
        // m_PatchHeights[5] = computeHeightFor2DPoint(0.333f, 0.333f);
        // m_PatchHeights[6] = computeHeightFor2DPoint(0.666f, 0.333f);
        // m_PatchHeights[7] = computeHeightFor2DPoint(1.0f, 0.333f);

        // m_PatchHeights[8] = computeHeightFor2DPoint(0.0f, 0.666f);
        // m_PatchHeights[9] = computeHeightFor2DPoint(0.333f, 0.666f);
        // m_PatchHeights[10] = computeHeightFor2DPoint(0.666f, 0.666f);
        // m_PatchHeights[11] = computeHeightFor2DPoint(1.0f, 0.666f);

        m_PatchHeights[12] = computeHeightFor2DPoint(0.0f, 1.0f);
        // m_PatchHeights[13] = computeHeightFor2DPoint(0.333f, 1.0f);
        // m_PatchHeights[14] = computeHeightFor2DPoint(0.666f, 1.0f);
        m_PatchHeights[15] = computeHeightFor2DPoint(1.0f, 1.0f);
    }

    void TerrainNode::update(const vd::core::CameraPtr& cameraPtr) {
        if (!(cameraPtr->isCameraRotated() || cameraPtr->isCameraMoved())) {
            return;
        }

        // Set this node as leaf
        m_Leaf = true;
        m_Children.clear();

        // If this node's level is greater than the maximum level, return
        // TODO: Read this constant from the config
        if (m_kLevel >= 7) {
            return;
        }

        // Compute if this node exceeds his level of detail threshold
        const auto levelMaxRange = float(m_kConfigPtr->getLodRange()[m_kLevel]);

        // Compute distance from each point to get the minimum distance from the patch to the camera position
        const auto& cameraPosition = cameraPtr->getPosition();
        float distanceBottomRight = distanceToCamera(cameraPosition, m_kBottomRight);
        float distanceBottomLeft = distanceToCamera(cameraPosition, glm::vec2(m_kTopLeft.x, m_kBottomRight.y));
        float distanceTopRight = distanceToCamera(cameraPosition, glm::vec2(m_kBottomRight.x, m_kTopLeft.y));
        float distanceTopLeft = distanceToCamera(cameraPosition, m_kTopLeft);

        float distance = std::min(std::min(distanceBottomRight, distanceBottomLeft),
                                  std::min(distanceTopRight, distanceTopLeft));

        // If the distance is not lower than the current level maximum range, return
        if (distance > levelMaxRange) {
            return;
        }

        // Else, set this node as being a parent, and create its children
        m_Leaf = false;

        m_Children.resize(4);

        // Top Left child
        m_Children[0] = std::make_shared<TerrainNode>(m_kConfigPtr,
                                                      m_kTopLeft,
                                                      m_kCenter,
                                               m_kLevel + 1);

        // Top Right child
        m_Children[1] = std::make_shared<TerrainNode>(m_kConfigPtr,
                                                      glm::vec2(m_kCenter.x, m_kTopLeft.y),
                                                      glm::vec2(m_kBottomRight.x, m_kCenter.y),
                                               m_kLevel + 1);

        // Bottom Left child
        m_Children[2] = std::make_shared<TerrainNode>(m_kConfigPtr,
                                                      glm::vec2(m_kTopLeft.x, m_kCenter.y),
                                                      glm::vec2(m_kCenter.x, m_kBottomRight.y),
                                               m_kLevel + 1);

        // Bottom Right child
        m_Children[3] = std::make_shared<TerrainNode>(m_kConfigPtr,
                                                      m_kCenter,
                                                      m_kBottomRight,
                                               m_kLevel + 1);

        // Update all children
        for (auto& child : m_Children) {
            child->update(cameraPtr);
        }
    }

    const TerrainNodePtrVec& TerrainNode::GetNodes() const {
        return m_Children;
    }

    bool TerrainNode::IsLeaf() const {
        return m_Leaf;
    }

    int TerrainNode::GetLevel() const {
        return m_kLevel;
    }

    const vd::math::Transform& TerrainNode::GetTransform() const {
        return m_Transform;
    }

    const TerrainNode::PatchHeightVec &TerrainNode::GetPatchHeights() const {
        return m_PatchHeights;
    }

    float TerrainNode::distanceToCamera(const glm::vec3& cameraPosition, const glm::vec2& localCoords) {
        const auto& heightImg = m_kConfigPtr->getHeightImg();
        const auto& worldTransform = m_kConfigPtr->getTransform();

        const auto height = vd::img::ImageHelper::texture(*heightImg, localCoords).r;

        // TODO: Optimization: Instead of transforming each point of the patch into world system, why not transforming
        //  camera position into local system once?
        glm::vec3 worldCoords = glm::vec3(worldTransform * glm::vec4(localCoords.x, height, localCoords.y, 1.0f));

        return glm::length(cameraPosition - worldCoords);
    }

    float TerrainNode::computeHeightFor2DPoint(float x, float z) const {
        const auto& heightImg = m_kConfigPtr->getHeightImg();

        glm::vec4 localSystemPoint = m_Transform * glm::vec4(x, 0.0f, z, 1.0f);
        glm::vec2 localCoords(localSystemPoint.x, localSystemPoint.z);

        auto scaleY = m_kConfigPtr->getScaleY();
        const auto height = vd::img::ImageHelper::texture(*heightImg, localCoords).r * scaleY;

        return height;
    }

}