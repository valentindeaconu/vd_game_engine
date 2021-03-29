//
// Created by Vali on 3/26/2021.
//

#ifndef VDGE_BIOMEMANAGER_HPP
#define VDGE_BIOMEMANAGER_HPP

#include <memory>
#include <unordered_map>

#include <engine/component/IManager.hpp>

#include <engine/loader/PropertiesLoader.hpp>
#include <engine/service/TextureService.hpp>
#include <engine/defines/Types.hpp>

#include <engine/injector/Injectable.hpp>
#include <modules/terrain/Terrain.hpp>

#include "Biome.hpp"
#include "splatmap/SplatMapBuilder.hpp"

#include <modules/terrain/TerrainRenderer.hpp>

namespace mod::biomes {
    class BiomeManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        static const vd::datastruct::Observable::priority_t kPriority = terrain::TerrainRenderer::kPriority + 1;

        BiomeManager(const std::string& propsFilePath);

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        void Populate(const std::unordered_map<std::string, vd::object::Entity3DPtr>& base);

        [[nodiscard]] const vd::gl::Texture2DPtr& SplatMap() const;
        [[nodiscard]] size_t BiomesCount() const;

        BiomePtr BiomeByIndex(size_t index) const;
        BiomePtr BiomeByName(const std::string& name) const;

        void ForEach(const vd::Setter<Biome>& setterFn);
        void ForEach(const vd::User<Biome>& userFn) const;

        [[nodiscard]] BiomePtrVec BiomesAt(float x, float z) const;
    private:
        std::vector<vd::Consumer>               m_InitFns;

        BiomePtrVec                             m_Biomes;
        std::unordered_map<std::string, size_t> m_NameToIndex;
        std::vector<std::vector<std::string>>   m_IndexToEntities;

        vd::model::ImagePtr<float, vd::model::ImageFormat::eRGBA> m_pSplatImg;
        vd::gl::Texture2DPtr m_pSplatMap;

        vd::property::PropertiesPtr m_pProperties;
        mod::terrain::TerrainPtr    m_pTerrain;
    };
    typedef std::shared_ptr<BiomeManager>   BiomeManagerPtr;
}

#endif //VDGE_BIOMEMANAGER_HPP