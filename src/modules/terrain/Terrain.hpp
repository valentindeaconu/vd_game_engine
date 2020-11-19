//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <engine/loader/PropertiesLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>

#include <memory>
#include <vector>
#include <cmath>

#include "Biome.hpp"
#include "TerrainNode.hpp"
#include "normalmap/NormalMapRenderer.hpp"
#include "splatmap/SplatMapRenderer.hpp"

namespace mod::terrain {
    class Terrain : public vd::object::Entity, public vd::injector::Injectable {
    public:
        explicit Terrain(const std::string& propsFilePath);
        ~Terrain();

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::property::PropertiesPtr& Properties() const;

        [[nodiscard]] const std::vector<TerrainNode::ptr_type_t>& RootNodes() const;

        [[nodiscard]] const BiomePtrVec& Biomes() const;

        [[nodiscard]] const vd::gl::Texture2DPtr& HeightMap() const;
        [[nodiscard]] const vd::gl::Texture2DPtr& NormalMap() const;
        [[nodiscard]] const vd::gl::Texture2DPtr& SplatMap() const;

        [[nodiscard]] float HeightAt(float x, float z) const;
        [[nodiscard]] BiomePtrVec BiomesAt(float x, float z) const;
    private:
        void CreateProps();
        void PopulateBiomes();
        void ComputeMaps();
        void PopulateBiomeWithObjects(BiomePtr& biomePtr, const std::string& biomePrefix);

        void GeneratePatch();
        void PopulateTree(const TerrainNode::ptr_type_t& root);

        vd::property::PropertiesPtr m_pProps;

        // Camera required for update optimization
        vd::camera::CameraPtr m_pCamera;

        // Level of detail nodes
        TerrainNode::ptr_type_t                 m_RootNode;
        std::vector<TerrainNode::ptr_type_t>    m_ImaginaryRootNodes;

        // Biomes
        BiomePtrVec m_Biomes;

        // Maps
        vd::model::ImagePtr<float, vd::model::ImageFormat::eR> m_pHeightImg;
        vd::gl::Texture2DPtr m_pHeightMap;

        vd::gl::Texture2DPtr m_pNormalMap;

        vd::model::ImagePtr<uint32_t, vd::model::ImageFormat::eR> m_pSplatImg;
        vd::gl::Texture2DPtr m_pSplatMap;

        // Level of detail ranges
        std::vector<int> m_LevelOfDetailRanges;

    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
