//
// Created by Vali on 3/26/2021.
//

#include "BiomeManager.hpp"

namespace mod::biomes {
    
    BiomeManager::BiomeManager(const std::string& propsFilePath) 
        : m_pProperties(vd::loader::PropertiesLoader::Load(propsFilePath))
    {
        for (int i = 0; ; ++i) {
            if (m_pProperties->Has("Biome." + std::to_string(i) + ".Name")) {
                m_Biomes.emplace_back(nullptr);
            } else {
                break;
            }
        }
    }

    void BiomeManager::Link() {
        m_pTerrain = vd::ObjectOfType<mod::terrain::Terrain>::Find();
    }

    void BiomeManager::Init() {
        for (int i = 0; i < m_Biomes.size(); ++i) {
            const std::string prefix = "Biome." + std::to_string(i);

            BiomePtr pBiome = std::make_shared<Biome>();

            pBiome->Name() = m_pProperties->Get<std::string>(prefix + ".Name");
            pBiome->MinimumHeight() = m_pProperties->Get<float>(prefix + ".MinHeight");
            pBiome->MaximumHeight() = m_pProperties->Get<float>(prefix + ".MaxHeight");
            pBiome->MinimumSlope() = m_pProperties->Get<float>(prefix + ".MinSlope");
            pBiome->MaximumSlope() = m_pProperties->Get<float>(prefix + ".MaxSlope");

            const std::string materialPrefix = prefix + ".Material";
            pBiome->Material().DiffuseMap() =
                    vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Diffuse"));
            pBiome->Material().DiffuseMap()->Bind();
            pBiome->Material().DiffuseMap()->MipmapLinearFilter();
            pBiome->Material().DiffuseMap()->Unbind();

            pBiome->Material().NormalMap() =
                    vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Normal"));
            pBiome->Material().NormalMap()->Bind();
            pBiome->Material().NormalMap()->MipmapLinearFilter();
            pBiome->Material().NormalMap()->Unbind();

            pBiome->Material().DisplaceMap() =
                    vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Displace"));
            pBiome->Material().DisplaceMap()->Bind();
            pBiome->Material().DisplaceMap()->MipmapLinearFilter();
            pBiome->Material().DisplaceMap()->Unbind();

            pBiome->Material().DisplaceScale() = m_pProperties->Get<float>(materialPrefix + ".HeightScaling");
            pBiome->Material().HorizontalScale() = m_pProperties->Get<float>(materialPrefix + ".HorizontalScaling");

            m_NameToIndex[pBiome->Name()] = i;
            m_Biomes[i] = std::move(pBiome);
            auto& entities = m_IndexToEntities.emplace_back();

            for (int j = 0; ; ++j) {
                try {
                    const std::string objectName = prefix + ".Object." + std::to_string(j);
                    entities.emplace_back(m_pProperties->Get<std::string>(objectName));
                } catch (std::invalid_argument& e) {
                    break;
                }
            }
        }

        // Compute SplatMap
        splatmap::SplatMapBuilder SMBuilder;
        SMBuilder.Create(
            m_pTerrain->HeightMap(), 
            m_pTerrain->NormalMap(), 
            int(m_pTerrain->HeightMap()->Width()),  
            m_Biomes, 
            m_pSplatMap, 
            m_pSplatImg
        );
        SMBuilder.CleanUp();
    }

    void BiomeManager::Update() {
        
    }
    
    void BiomeManager::CleanUp() {

    }

    void BiomeManager::Populate(const std::unordered_map<std::string, vd::object::Entity3DPtr>& base) {
        for (int i = 0; i < m_Biomes.size(); ++i) {
            auto& pBiome = m_Biomes[i];
            auto& entityNames = m_IndexToEntities[i];

            for (auto& name : entityNames) {
                auto it = base.find(name);
                if (it != base.end()) {
                    pBiome->Entities().emplace_back(it->second);
                } else {
                    vd::Logger::warn("Entity with name " + name + " could not be found");
                }
            }
        }
    }

    const vd::gl::Texture2DPtr& BiomeManager::SplatMap() const {
        return m_pSplatMap;
    }

    size_t BiomeManager::BiomesCount() const {
        return m_Biomes.size();
    }

    BiomePtr BiomeManager::BiomeByIndex(size_t index) const {
        if (index >= m_Biomes.size()) {
            return nullptr;
        }

        return m_Biomes[index];
    }

    BiomePtr BiomeManager::BiomeByName(const std::string& name) const {
        auto it = m_NameToIndex.find(name);
        if (it != m_NameToIndex.end()) {
            return m_Biomes[it->second];
        }

        return nullptr;
    }

    void BiomeManager::ForEach(const vd::Setter<Biome>& setterFn) {
        for (const auto& pBiome : m_Biomes) {
            setterFn(*pBiome);
        }
    }

    void BiomeManager::ForEach(const vd::User<Biome>& userFn) const {
        for (const auto& pBiome : m_Biomes) {
            userFn(*pBiome);
        }
    }


    BiomePtrVec BiomeManager::BiomesAt(float x, float z) const {
        glm::vec2 uv = m_pTerrain->ToTerrainUV(z, x);

        if (uv.x < 0.0f || uv.y < 0.0f) {
            return BiomePtrVec();
        }

        // SplatMap is a texture atlas, with 2x2 textures
        uv *= 0.5f;

        std::array<glm::vec4, 4> blendSample;
        // top left
        blendSample[0] = m_pSplatImg->Get<glm::vec4, vd::math::Interpolation::eBilinear>(glm::vec2(uv.y, uv.x));
        // top right
        blendSample[1] = m_pSplatImg->Get<glm::vec4, vd::math::Interpolation::eBilinear>(glm::vec2(uv.y, uv.x + .5f));
        // bottom left
        blendSample[2] = m_pSplatImg->Get<glm::vec4, vd::math::Interpolation::eBilinear>(glm::vec2(uv.y + .5f, uv.x));
        // bottom right
        blendSample[3] = m_pSplatImg->Get<glm::vec4, vd::math::Interpolation::eBilinear>(glm::vec2(uv.y + .5f, uv.x + .5f));

        BiomePtrVec output;
        for (int k = 0; k < m_Biomes.size(); ++k) {
            int sampleIndex = k >> 2; // k / 4
            int channelIndex = k % 4;

            if (blendSample[sampleIndex][channelIndex] > .0f) {
                output.emplace_back(m_Biomes[k]);
            }
        }

        return output;
    }
}