//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <engine/misc/Properties.hpp>

#include <engine/camera/Camera.hpp>
#include <engine/kernel/ObjectOfType.hpp>

#include <memory>
#include <vector>
#include <cmath>

#include "Biome.hpp"
#include "TerrainNode.hpp"
#include "normalmap/NormalMapRenderer.hpp"
#include "splatmap/SplatMapRenderer.hpp"

namespace mod::terrain {
    class Terrain : public vd::object::Entity {
    public:
        explicit Terrain(const std::string& propsFilePath);
        ~Terrain();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::misc::PropertiesPtr& GetProperties() const;

        [[nodiscard]] const std::vector<TerrainNode::ptr_type_t>& GetRootNodes() const;

        [[nodiscard]] const BiomePtrVec& GetBiomes() const;

        [[nodiscard]] const vd::gl::Texture2DPtr& GetHeightMap() const;
        [[nodiscard]] const vd::gl::Texture2DPtr& GetNormalMap() const;
        [[nodiscard]] const vd::gl::Texture2DPtr& GetSplatMap() const;

        [[nodiscard]] float GetHeight(float x, float z) const;
        [[nodiscard]] BiomePtrVec GetBiomesAt(float x, float z) const;
    private:
        void CreateProps();
        void PopulateBiomes();
        void ComputeMaps();
        void PopulateBiomeWithObjects(BiomePtr& biomePtr, const std::string& biomePrefix);

        void GeneratePatch();
        void PopulateTree(const TerrainNode::ptr_type_t& root);

        vd::misc::PropertiesPtr m_PropsPtr;

        // Camera required for update optimization
        vd::camera::CameraPtr m_CameraPtr;

        // Level of detail nodes
        TerrainNode::ptr_type_t m_RootNode;
        std::vector<TerrainNode::ptr_type_t> m_ImaginaryRootNodes;

        // Biomes
        BiomePtrVec m_Biomes;

        // Maps
        vd::model::ImagePtr<float, vd::model::ImageFormat::eR> m_HeightImg;
        vd::gl::Texture2DPtr m_HeightMap;

        vd::gl::Texture2DPtr m_NormalMap;

        vd::model::ImagePtr<uint32_t, vd::model::ImageFormat::eR> m_SplatImg;
        vd::gl::Texture2DPtr m_SplatMap;

        // Level of detail ranges
        std::vector<int> m_LevelOfDetailRanges;

    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
