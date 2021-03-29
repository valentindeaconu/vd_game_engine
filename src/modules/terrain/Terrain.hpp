//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity2D.hpp>

#include <engine/loader/PropertiesLoader.hpp>
#include <engine/service/TextureService.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>

#include <memory>
#include <vector>
#include <cmath>

#include <modules/props/Prop.hpp>

#include "TerrainNode.hpp"
#include "normalmap/NormalMapBuilder.hpp"

namespace mod::terrain {
    class Terrain : public vd::object::Entity2D, public vd::injector::Injectable {
    public:
        explicit Terrain(const std::string& propsFilePath);

        void Link() override;

        void Setup() override;
        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] float MapSize() const;
        [[nodiscard]] glm::vec2 Radius() const;
        [[nodiscard]] const glm::vec2& Center() const;

        [[nodiscard]] const vd::property::PropertiesPtr& Properties() const;

        [[nodiscard]] const std::vector<TerrainNode::ptr_type_t>& RootNodes() const;

        [[nodiscard]] const vd::gl::Texture2DPtr& HeightMap() const;
        [[nodiscard]] const vd::gl::Texture2DPtr& NormalMap() const;

        [[nodiscard]] glm::vec2 ToTerrainUV(float x, float z) const;

        [[nodiscard]] float HeightAt(float x, float z) const;
    private:
        void ComputeCenterAndRadius();
        void CreateProps();
        void ComputeMaps();

        std::vector<glm::vec2> GeneratePatch();
        void PopulateTree(const TerrainNode::ptr_type_t& root);

        vd::property::PropertiesPtr m_pProperties;

        // Map Size
        float m_MapSize;
        float m_ScaleY;

        // Circular Shape
        glm::vec2   m_Radius;
        glm::vec2   m_Center;

        // Camera required for update optimization
        vd::camera::CameraPtr m_pCamera;

        // Level of detail nodes
        TerrainNode::ptr_type_t                 m_RootNode;
        std::vector<TerrainNode::ptr_type_t>    m_ImaginaryRootNodes;

        // Maps
        vd::model::ImagePtr<float, vd::model::ImageFormat::eR> m_pHeightImg;
        vd::gl::Texture2DPtr m_pHeightMap;

        vd::gl::Texture2DPtr m_pNormalMap;

        // Level of detail ranges
        std::vector<int> m_LevelOfDetailRanges;

    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
