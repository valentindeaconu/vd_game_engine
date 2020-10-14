//
// Created by Vali on 10/6/2020.
//

#include "TerrainNode.hpp"

namespace mod::terrain {
    TerrainNode::TerrainNode(const TerrainNode* parent,
                             const TerrainConfigPtr& terrainConfigPtr,
                             const glm::vec2& topLeft,
                             const glm::vec2& bottomRight,
                             int level,
                             NodeIndex nodeIndex)
            : m_kParent(parent)
            , m_kConfigPtr(terrainConfigPtr)
            , m_kTopLeft(topLeft)
            , m_kBottomRight(bottomRight)
            , m_kLevel(level)
            , m_Leaf(true)
            , m_kCenter((topLeft.x + bottomRight.x) / 2.0f, (topLeft.y + bottomRight.y) / 2.0f)
            , m_TessFactor(1.0f)
            , m_kNodeIndex(nodeIndex)
    {
        assert((bottomRight.x - topLeft.x) == (bottomRight.y - topLeft.y));

        m_Transform.setTranslation(topLeft.x, 0.0f, topLeft.y);
        m_Transform.setScaling(bottomRight.x - topLeft.x, 1.0f, bottomRight.x - topLeft.x);

        const auto scaleXZ = terrainConfigPtr->getScaleXZ();
        const auto rtRootNodes = float(std::sqrt(terrainConfigPtr->getRootNodes()));
        const auto levelDivision = float(1 << level);

        m_EdgeLength = scaleXZ / (rtRootNodes * levelDivision);

        computeEdgeMiddles();
    }

    void TerrainNode::Update(const vd::core::CameraPtr& cameraPtr) {
        if ((m_kConfigPtr->isLevelOfDetailEnabled()) && (cameraPtr->isCameraRotated() || cameraPtr->isCameraMoved())) {
            // Set this node as leaf
            m_Leaf = true;
            m_Children.clear();

            // If this node's level is greater or equal to the maximum level, then just ignore it
            if (m_kLevel < m_kConfigPtr->getMaxDetailLevel()) {
                // Compute if this node exceeds his level of detail threshold
                const auto levelMinRange = float(m_kConfigPtr->getLodRange()[m_kLevel]);

                // Compute distance from each point to get the minimum distance from the patch to the camera position
                const auto &cameraPosition = cameraPtr->getPosition();

                glm::vec4 distance;
                float nodeDistance;
                for (int i = 0; i < 4; ++i) {
                    distance[i] = glm::length(cameraPosition - m_EdgeMid[i]);
                    nodeDistance = (i == 0) ? distance[i] : std::min(nodeDistance, distance[i]);
                }

                // If the distance is less than the current level minimum range, this node should divide
                if (nodeDistance <= levelMinRange) {
                    // Set this node as being a parent, and create its children
                    m_Leaf = false;
                    computeChildren();

                    // Update all children
                    for (auto &childPtr : m_Children) {
                        childPtr->Update(cameraPtr);
                    }
                }
            }
        }
    }

    void TerrainNode::UpdateNeighbours() {
        // Update neighbours only if this node is not a root node (root node neighbours never changes!)
        // Also, there's no point in computing neighbours if this node is not a leaf, because the responsability
        // to check the border is passed to the children
        if (m_kNodeIndex != eRootNode && m_Leaf) {
            computeNeighbours();

            // Check if any neighbour has a different level of detail
            // In that case, compute the difference between levels and set tessellation factor on that edge
            // to be equal to 2 to the power of the difference

            if (m_Neighbours[0] == nullptr && m_kLevel >= 1) {
                int a = 0;
                ++a;
            }

            // TODO: What happens if the tessellation factor goes over limit (gl_MaxTessLevel)?
            //  Is that possible?
            for (int i = 0; i < 4; ++i) {
                if (m_Neighbours[i] != nullptr && m_Neighbours[i]->GetLevel() >= m_kLevel) {
                    int count = m_Neighbours[i]->GetLevel() - m_kLevel;
                    for (const TerrainNode *r = m_Neighbours[i]; !r->IsLeaf(); r = r->GetNodes()[0].get())
                        ++count;
                    m_TessFactor[i] = float(1 << count);
                }
            }
        } else {
            // Update all children' neighbours
            for (auto& childPtr : m_Children) {
                childPtr->UpdateNeighbours();
            }
        }
    }

    const TerrainNodePtrVec& TerrainNode::GetNodes() const {
        return m_Children;
    }

    const glm::vec2& TerrainNode::GetTopLeft() const {
        return m_kTopLeft;
    }

    int TerrainNode::GetLevel() const {
        return m_kLevel;
    }

    bool TerrainNode::IsLeaf() const {
        return m_Leaf;
    }

    const vd::math::Transform& TerrainNode::GetTransform() const {
        return m_Transform;
    }

    const TerrainNode::PointVec& TerrainNode::GetEdgeMiddles() const {
        return m_EdgeMid;
    }

    const glm::vec4& TerrainNode::GetTessFactors() const {
        return m_TessFactor;
    }

    const TerrainNode::NodeIndex& TerrainNode::GetIndex() const {
        return m_kNodeIndex;
    }

    void TerrainNode::SetNeighbour(const TerrainNodePtr& neighbour, EdgeIndex neighbourIndex) {
        m_Neighbours[neighbourIndex] = neighbour.get();
    }

    void TerrainNode::computeEdgeMiddles() {
        const auto& worldTransform = m_kConfigPtr->getTransform();
        const auto& heightImg = m_kConfigPtr->getHeightImg();

        auto convertToWorldCoords = [&](float x, float y) {
            const auto h = vd::img::ImageHelper::texture(*heightImg, glm::vec2(x, y)).r;
            return glm::vec3(worldTransform * glm::vec4(x, h, y, 1.0f));
        };

        m_EdgeMid[eTop] = convertToWorldCoords(m_kCenter.x, m_kTopLeft.y);
        m_EdgeMid[eRight] = convertToWorldCoords(m_kBottomRight.x, m_kCenter.y);
        m_EdgeMid[eBottom] = convertToWorldCoords(m_kCenter.x, m_kBottomRight.y);
        m_EdgeMid[eLeft] = convertToWorldCoords(m_kTopLeft.x, m_kCenter.y);
    }

    void TerrainNode::computeChildren() {
        if (!m_Children.empty())
            m_Children.clear();

        m_Children.resize(4);

        // Top Left child
        m_Children[eTopLeft] = std::make_shared<TerrainNode>(this,
                                                             m_kConfigPtr,
                                                             m_kTopLeft,
                                                             m_kCenter,
                                                             m_kLevel + 1,
                                                             eTopLeft);

        // Top Right child
        m_Children[eTopRight] = std::make_shared<TerrainNode>(this,
                                                              m_kConfigPtr,
                                                              glm::vec2(m_kCenter.x, m_kTopLeft.y),
                                                              glm::vec2(m_kBottomRight.x, m_kCenter.y),
                                                              m_kLevel + 1,
                                                              eTopRight);

        // Bottom Left child
        m_Children[eBottomLeft] = std::make_shared<TerrainNode>(this,
                                                                m_kConfigPtr,
                                                                glm::vec2(m_kTopLeft.x, m_kCenter.y),
                                                                glm::vec2(m_kCenter.x, m_kBottomRight.y),
                                                                m_kLevel + 1,
                                                                eBottomLeft);

        // Bottom Right child
        m_Children[eBottomRight] = std::make_shared<TerrainNode>(this,
                                                                 m_kConfigPtr,
                                                                 m_kCenter,
                                                                 m_kBottomRight,
                                                                 m_kLevel + 1,
                                                                 eBottomRight);
    }

    void TerrainNode::computeNeighbours() {
        if (m_kNodeIndex == eRootNode) {
            return;
        }

        const float localEdgeLength = m_kBottomRight.x - m_kTopLeft.x;

        const vd::math::Bounds2 northEstimatedBounds(glm::vec2(m_kTopLeft.x, m_kTopLeft.y - localEdgeLength),
                                                     glm::vec2(m_kBottomRight.x, m_kTopLeft.y));

        const vd::math::Bounds2 eastEstimatedBounds(glm::vec2(m_kBottomRight.x, m_kTopLeft.y),
                                                    glm::vec2(m_kBottomRight.x + localEdgeLength, m_kBottomRight.y));

        const vd::math::Bounds2 southEstimatedBounds(glm::vec2(m_kTopLeft.x, m_kBottomRight.y),
                                                     glm::vec2(m_kBottomRight.x, m_kBottomRight.y  + localEdgeLength));

        const vd::math::Bounds2 westEstimatedBounds(glm::vec2(m_kTopLeft.x - localEdgeLength, m_kTopLeft.y),
                                                    glm::vec2(m_kTopLeft.x, m_kBottomRight.y));


        switch (m_kNodeIndex) {
            case eTopLeft: {
                m_Neighbours[eRight] = m_kParent->GetNodes()[eTopRight].get();
                m_Neighbours[eBottom] = m_kParent->GetNodes()[eBottomLeft].get();

                m_Neighbours[eLeft] = searchNeighbour(m_kParent->m_kParent, westEstimatedBounds, true, m_kNodeIndex);
                m_Neighbours[eTop] = searchNeighbour(m_kParent->m_kParent, northEstimatedBounds, true, m_kNodeIndex);
                break;
            }
            case eTopRight: {
                m_Neighbours[eLeft] = m_kParent->GetNodes()[eTopLeft].get();
                m_Neighbours[eBottom] = m_kParent->GetNodes()[eBottomRight].get();

                m_Neighbours[eRight] = searchNeighbour(m_kParent->m_kParent, eastEstimatedBounds, true, m_kNodeIndex);
                m_Neighbours[eTop] = searchNeighbour(m_kParent->m_kParent, northEstimatedBounds, true, m_kNodeIndex);
                break;
            }
            case eBottomLeft: {
                m_Neighbours[eRight] = m_kParent->GetNodes()[eBottomRight].get();
                m_Neighbours[eTop] = m_kParent->GetNodes()[eTopLeft].get();

                m_Neighbours[eLeft] = searchNeighbour(m_kParent->m_kParent, westEstimatedBounds, true, m_kNodeIndex);
                m_Neighbours[eBottom] = searchNeighbour(m_kParent->m_kParent, southEstimatedBounds, true, m_kNodeIndex);
                break;
            }
            case eBottomRight: {
                m_Neighbours[eLeft] = m_kParent->GetNodes()[eBottomLeft].get();
                m_Neighbours[eTop] = m_kParent->GetNodes()[eTopRight].get();

                m_Neighbours[eRight] = searchNeighbour(m_kParent->m_kParent, eastEstimatedBounds, true, m_kNodeIndex);
                m_Neighbours[eBottom] = searchNeighbour(m_kParent->m_kParent, southEstimatedBounds, true, m_kNodeIndex);
                break;
            }
        }
    }

    TerrainNode::MatchingResult TerrainNode::containsSquare(const vd::math::Bounds2& bounds) const {
        if (bounds.IsEmpty()) {
            glm::vec2 point = bounds.GetLeft();

            if (point == m_kTopLeft && point == m_kBottomRight) {
                return ePerfectMatch;
            } else if (point.x >= m_kTopLeft.x && point.x <= m_kBottomRight.x &&
                       point.y >= m_kTopLeft.y && point.y <= m_kBottomRight.y) {
                return eInside;
            }

            return eOutside;
        }

        if (!bounds.IsValid()) {
            return eOutside;
        }

        if (bounds.GetLeft() == m_kTopLeft && bounds.GetRight() == m_kBottomRight) {
            return ePerfectMatch;
        }

        if (bounds.GetLeft().x >= m_kTopLeft.x &&
            bounds.GetRight().x <= m_kBottomRight.x &&
            bounds.GetLeft().y >= m_kTopLeft.y &&
            bounds.GetRight().y <= m_kBottomRight.y) {
            return eInside;
        }

        return eOutside;
    }

    const TerrainNode* TerrainNode::searchNeighbour(const TerrainNode* nodePtr,
                                                    const vd::math::Bounds2& searchingBounds,
                                                    bool parentSearch,
                                                    NodeIndex caller) {
        if (nodePtr == nullptr) {
            return nullptr;
        }

        switch (nodePtr->containsSquare(searchingBounds)) {
            case eOutside: return nullptr;
            case ePerfectMatch: return nodePtr;
            case eInside:
                if (nodePtr->IsLeaf()) {
                    return nodePtr;
                }
                break;
        }

        for (int nodeIndex = 0; nodeIndex < 4; ++nodeIndex) {
            if (caller == eRootNode || caller != nodeIndex) {
                const auto& childPtr = nodePtr->GetNodes()[nodeIndex];
                if (childPtr != nullptr) {
                    auto resultPtr = searchNeighbour(childPtr, searchingBounds);
                    if (resultPtr != nullptr) {
                        return resultPtr;
                    }
                }
            }
        }

        if (parentSearch && nodePtr->m_kParent != nullptr) {
            return searchNeighbour(nodePtr->m_kParent, searchingBounds, true, nodePtr->m_kNodeIndex);
        }

        return nullptr;
    }

    const TerrainNode* TerrainNode::searchNeighbour(const TerrainNodePtr& nodePtr,
                                                    const vd::math::Bounds2& searchingBounds,
                                                    bool parentSearch,
                                                    NodeIndex caller) {
        return searchNeighbour(nodePtr.get(), searchingBounds, parentSearch, caller);
    }
}