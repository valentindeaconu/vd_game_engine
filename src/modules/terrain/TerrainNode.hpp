//
// Created by Vali on 10/6/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINNODE_HPP
#define VD_GAME_ENGINE_TERRAINNODE_HPP

#include <engine/camera/Camera.hpp>
#include <engine/math/Transform.hpp>
#include <engine/math/Bounds.hpp>
#include <engine/misc/Properties.hpp>

#include <engine/datastruct/Tree.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <array>

namespace mod::terrain {
    class TerrainNode : public vd::datastruct::Quadtree {
    public:
        typedef std::shared_ptr<TerrainNode>   ptr_type_t;

        enum NodeIndex {
            eRootNode = -1,
            eTopLeft = 0,
            eTopRight,
            eBottomLeft,
            eBottomRight,
        };

        enum EdgeIndex {
            eTop = 0,
            eRight,
            eBottom,
            eLeft
        };

        typedef std::array<glm::vec3, 4> PointVec;

        typedef std::function<glm::vec3(float, float)> WorldCoordinatesConvertor;

        TerrainNode(const TerrainNode* parent,
                    const glm::vec2& topLeft,
                    const glm::vec2& bottomRight,
                    const WorldCoordinatesConvertor& worldCoordinatesConvertor,
                    int maxLevel,
                    const std::vector<int>* lodRangesPtr,
                    int level,
                    NodeIndex nodeIndex);

        void Update(const vd::camera::CameraPtr& cameraPtr);
        void UpdateNeighbours();

        void Populate() override;
        
        [[nodiscard]] const glm::vec2& GetTopLeft() const;

        [[nodiscard]] const vd::math::Transform& GetTransform() const;

        [[nodiscard]] const PointVec& GetEdgeMiddles() const;

        [[nodiscard]] const glm::vec4& GetTessFactors() const;
    private:

        void ComputeEdgeMiddles();
        void ComputeNeighbours();

        enum MatchingResult {
            eOutside = 0,
            eInside,
            ePerfectMatch
        };
        [[nodiscard]] MatchingResult ContainsSquare(const vd::math::Bounds2& searchingBounds) const;

        static const TerrainNode* SearchNeighbour(const TerrainNode* nodePtr,
                                                  const vd::math::Bounds2& searchingBounds,
                                                  bool parentSearch = false,
                                                  NodeIndex caller = eRootNode);

        WorldCoordinatesConvertor m_WorldCoordinatesConvertor;

        const int m_kMaxLevelOfDetail;
        const std::vector<int>* m_kLevelOfDetailRangesPtr;

        const glm::vec2 m_kTopLeft;
        const glm::vec2 m_kBottomRight;
        const glm::vec2 m_kCenter;

        PointVec m_EdgeMid;

        glm::vec4 m_TessFactor;

        vd::math::Transform m_Transform;

        std::array<const TerrainNode*, 4> m_Neighbours;
    };
}


#endif //VD_GAME_ENGINE_TERRAINNODE_HPP
