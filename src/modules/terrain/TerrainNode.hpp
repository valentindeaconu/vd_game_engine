//
// Created by Vali on 10/6/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINNODE_HPP
#define VD_GAME_ENGINE_TERRAINNODE_HPP

#include <engine/core/Camera.hpp>
#include <engine/foundation/img/imghelper/ImageHelper.hpp>
#include <engine/foundation/math/Transform.hpp>
#include <engine/foundation/math/Bounds.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <array>

#include "TerrainConfig.hpp"

namespace mod::terrain {
    class TerrainNode;
    typedef std::shared_ptr<TerrainNode>   TerrainNodePtr;
    typedef std::vector<TerrainNodePtr>    TerrainNodePtrVec;

    class TerrainNode {
    public:
        enum NodeIndex {
            eTopLeft = 0,
            eTopRight,
            eBottomLeft,
            eBottomRight,
            eRootNode
        };

        enum EdgeIndex {
            eTop = 0,
            eRight,
            eBottom,
            eLeft
        };

        typedef std::array<glm::vec3, 4> PointVec;

        TerrainNode(const TerrainNode* parent,
                    const TerrainConfigPtr& terrainConfigPtr,
                    const glm::vec2& topLeft,
                    const glm::vec2& bottomRight,
                    int level,
                    NodeIndex nodeIndex);

        void Update(const vd::core::CameraPtr& cameraPtr);
        void UpdateNeighbours();

        [[nodiscard]] const TerrainNodePtrVec& GetNodes() const;

        [[nodiscard]] const glm::vec2& GetTopLeft() const;

        [[nodiscard]] int GetLevel() const;

        [[nodiscard]] bool IsLeaf() const;

        [[nodiscard]] const vd::math::Transform& GetTransform() const;

        [[nodiscard]] const PointVec& GetEdgeMiddles() const;

        [[nodiscard]] const glm::vec4& GetTessFactors() const;

        [[nodiscard]] const NodeIndex& GetIndex() const;

        void SetNeighbour(const TerrainNodePtr& neighbour, EdgeIndex neighbourIndex);
    private:
        void computeEdgeMiddles();
        void computeChildren();
        void computeNeighbours();

        enum MatchingResult {
            eOutside = 0,
            eInside,
            ePerfectMatch
        };
        [[nodiscard]] MatchingResult containsSquare(const vd::math::Bounds2& searchingBounds) const;

        static const TerrainNode* searchNeighbour(const TerrainNode* nodePtr,
                                                  const vd::math::Bounds2& searchingBounds,
                                                  bool parentSearch = false,
                                                  NodeIndex caller = eRootNode);
        static const TerrainNode* searchNeighbour(const TerrainNodePtr& nodePtr,
                                                  const vd::math::Bounds2& searchingBounds,
                                                  bool parentSearch = false,
                                                  NodeIndex caller = eRootNode);

        const TerrainNode* m_kParent;

        const NodeIndex m_kNodeIndex;

        const TerrainConfigPtr m_kConfigPtr;

        bool m_Leaf;

        const int m_kLevel;

        const glm::vec2 m_kTopLeft;
        const glm::vec2 m_kBottomRight;

        const glm::vec2 m_kCenter;

        PointVec m_EdgeMid;

        float m_EdgeLength;

        glm::vec4 m_TessFactor;

        TerrainNodePtrVec m_Children;

        vd::math::Transform m_Transform;

        std::array<const TerrainNode*, 4> m_Neighbours;
    };
}


#endif //VD_GAME_ENGINE_TERRAINNODE_HPP
