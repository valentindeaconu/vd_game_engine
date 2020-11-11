//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <engine/foundation/algorithm/TreeHelper.hpp>

#include <engine/misc/Properties.hpp>

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
        Terrain(const vd::EnginePtr& enginePtr, const std::string& propsFilePath);
        ~Terrain();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const vd::misc::PropertiesPtr& GetProperties() const;

        [[nodiscard]] const std::vector<TerrainNode::ptr_type_t>& GetRootNodes() const;

        [[nodiscard]] const BiomePtrVec& GetBiomes() const;

        [[nodiscard]] const vd::model::Texture2DPtr& GetHeightMap() const;
        [[nodiscard]] const vd::model::Texture2DPtr& GetNormalMap() const;
        [[nodiscard]] const vd::model::Texture2DPtr& GetSplatMap() const;

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

        // Level of detail nodes
        TerrainNode::ptr_type_t m_RootNode;
        std::vector<TerrainNode::ptr_type_t> m_ImaginaryRootNodes;

        // Biomes
        BiomePtrVec m_Biomes;

        // Maps
        vd::img::ImageFPtr m_HeightImg;
        vd::model::Texture2DPtr m_HeightMap;

        vd::model::Texture2DPtr m_NormalMap;

        vd::img::ImageIPtr m_SplatImg;
        vd::model::Texture2DPtr m_SplatMap;

        // Level of detail ranges
        std::vector<int> m_LevelOfDetailRanges;

    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP
