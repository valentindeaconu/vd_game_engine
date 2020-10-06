//
// Created by Vali on 10/6/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINNODE_HPP
#define VD_GAME_ENGINE_TERRAINNODE_HPP

#include <engine/core/Camera.hpp>
#include <engine/foundation/img/imghelper/ImageHelper.hpp>
#include <engine/foundation/math/Transform.hpp>

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
        typedef std::array<float, 16>   PatchHeightVec;

        TerrainNode(const TerrainConfigPtr& terrainConfigPtr,
                    const glm::vec2& topLeft,
                    const glm::vec2& bottomRight,
                    int level);

        void update(const vd::core::CameraPtr& cameraPtr);

        [[nodiscard]] const TerrainNodePtrVec& GetNodes() const;

        [[nodiscard]] bool IsLeaf() const;

        [[nodiscard]] int GetLevel() const;

        [[nodiscard]] const vd::math::Transform& GetTransform() const;

        [[nodiscard]] const PatchHeightVec& GetPatchHeights() const;

    private:
        float distanceToCamera(const glm::vec3& cameraPosition, const glm::vec2& localCoords);
        [[nodiscard]] float computeHeightFor2DPoint(float x, float z) const;

        const TerrainConfigPtr m_kConfigPtr;

        const glm::vec2 m_kTopLeft;
        const glm::vec2 m_kBottomRight;
        const int m_kLevel;
        bool m_Leaf;

        glm::vec2 m_kCenter;

        TerrainNodePtrVec m_Children;

        vd::math::Transform m_Transform;

        PatchHeightVec m_PatchHeights;
    };
}


#endif //VD_GAME_ENGINE_TERRAINNODE_HPP
