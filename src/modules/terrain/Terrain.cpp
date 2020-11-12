//
// Created by Vali on 9/21/2020.
//

#include "Terrain.hpp"

namespace mod::terrain {

    Terrain::Terrain(const std::string& propsFilePath)
        : m_PropsPtr(vd::misc::Properties::Create<vd::misc::Properties::eFile>(propsFilePath))
    {
    }

    Terrain::~Terrain() = default;

    void Terrain::Init() {
        m_CameraPtr = vd::ObjectOfType<vd::core::Camera>::Find();

        CreateProps();

        // Create world transform
        const auto scaleXZ = m_PropsPtr->Get<float>("ScaleXZ");
        const auto scaleY = m_PropsPtr->Get<float>("ScaleY");
        GetWorldTransform().SetScaling(scaleXZ, scaleY, scaleXZ);
        GetWorldTransform().SetTranslation(-scaleXZ / 2.0f, 0.0f, -scaleXZ / 2.0f);

        PopulateBiomes();

        ComputeMaps();

        auto convertToWorldCoords = [&](float x, float y) {
            const auto h = vd::img::ImageHelper::texture(*m_HeightImg, glm::vec2(x, y)).r;
            return glm::vec3(GetWorldTransform() * glm::vec4(x, h, y, 1.0f));
        };

        m_RootNode = std::make_shared<TerrainNode>(nullptr,
                                                   glm::vec2(0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f),
                                                   convertToWorldCoords,
                                                   m_PropsPtr->Get<int>("MaxLevelOfDetail"),
                                                   &m_LevelOfDetailRanges,
                                                   0,
                                                   TerrainNode::eRootNode);

        PopulateTree(m_RootNode);

        GeneratePatch();

        Entity::Init();
    }

    void Terrain::Update() {
        if (m_PropsPtr->Get<bool>("LevelOfDetailEnabled")) {
            if (m_CameraPtr->isCameraMoved() || m_CameraPtr->isCameraRotated()) {
                for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                    imaginaryRootNode->Update(m_CameraPtr);
                }

                for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                    imaginaryRootNode->UpdateNeighbours();
                }
            }
        }
    }

    void Terrain::CleanUp() {
        m_RootNode->Clear();
        m_ImaginaryRootNodes.clear();

        m_RootNode = nullptr;

        Entity::CleanUp();
    }

    const vd::misc::PropertiesPtr& Terrain::GetProperties() const {
        return m_PropsPtr;
    }

    const std::vector<TerrainNode::ptr_type_t>& Terrain::GetRootNodes() const {
        return m_ImaginaryRootNodes;
    }

    const BiomePtrVec& Terrain::GetBiomes() const {
        return m_Biomes;
    }

    const vd::model::Texture2DPtr& Terrain::GetHeightMap() const {
        return m_HeightMap;
    }

    const vd::model::Texture2DPtr& Terrain::GetNormalMap() const {
        return m_NormalMap;
    }

    const vd::model::Texture2DPtr& Terrain::GetSplatMap() const {
        return m_SplatMap;
    }

    float Terrain::GetHeight(float x, float z) const {
        const auto scaleXZ = m_PropsPtr->Get<float>("ScaleXZ");
        const auto scaleY = m_PropsPtr->Get<float>("ScaleY");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return 0.0f;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto height = vd::img::ImageHelper::texture(*m_HeightImg, glm::vec2(rz, rx)).r;

        return height * scaleY;
    }

    BiomePtrVec Terrain::GetBiomesAt(float x, float z) const {
        const auto scaleXZ = m_PropsPtr->Get<float>("ScaleXZ");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        BiomePtrVec output;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return output;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto mask = vd::img::ImageHelper::texture(*m_SplatImg, glm::vec2(rz, rx)).r;

        for (size_t k = 0; k < m_Biomes.size(); ++k) {
            if ((mask & (1 << k)) != 0) {
                output.emplace_back(m_Biomes[k]);
            }
        }

        return output;
    }

    void Terrain::CreateProps() {
        // Level offset (Jump over imaginary root nodes)
        const auto kRootNodes = m_PropsPtr->Get<int>("RootNodes");
        const int kLevelOffset = int(std::log(kRootNodes) / std::log(4));
        if (!m_PropsPtr->Set("LevelOffset", std::to_string(kLevelOffset))) {
            vd::Logger::terminate("Could not add level offset to the properties", 1);
        }
        // Max Level of detail
        for (int i = 0; ; ++i) {
            try {
                m_LevelOfDetailRanges.emplace_back(m_PropsPtr->Get<int>("Lod." + std::to_string(i) + ".Range"));
            } catch (std::invalid_argument& e) {
                if (!m_PropsPtr->Set("MaxLevelOfDetail", std::to_string(i))) {
                    vd::Logger::terminate("Could not add max level of detail to the properties", 1);
                }
                break;
            }
        }
    }

    void Terrain::PopulateBiomes() {
        const auto scaleY = m_PropsPtr->Get<float>("ScaleY");
        for (int i = 0; ; ++i) {
            try {
                const std::string prefix = "Biome." + std::to_string(i);

                BiomePtr biomePtr = std::make_shared<Biome>();

                biomePtr->setName(m_PropsPtr->Get<std::string>(prefix + ".Name"));
                biomePtr->setMinHeight(m_PropsPtr->Get<float>(prefix + ".MinHeight") * scaleY);
                biomePtr->setMaxHeight(m_PropsPtr->Get<float>(prefix + ".MaxHeight") * scaleY);

                const std::string materialPrefix = prefix + ".Material";
                vd::model::Material material;
                material.diffuseMap = vd::model::TextureService::get(m_PropsPtr->Get<std::string>(materialPrefix + ".Diffuse"));
                material.diffuseMap->bind();
                material.diffuseMap->trilinearFilter();
                material.diffuseMap->unbind();

                material.normalMap = vd::model::TextureService::get(m_PropsPtr->Get<std::string>(materialPrefix + ".Normal"));
                material.normalMap->bind();
                material.normalMap->bilinearFilter();
                material.normalMap->unbind();

                material.displaceMap = vd::model::TextureService::get(m_PropsPtr->Get<std::string>(materialPrefix + ".Displace"));
                material.displaceMap->bind();
                material.displaceMap->bilinearFilter();
                material.displaceMap->unbind();

                material.displaceScale = m_PropsPtr->Get<float>(materialPrefix + ".HeightScaling");
                material.horizontalScale = m_PropsPtr->Get<float>(materialPrefix + ".HorizontalScaling");
                biomePtr->setMaterial(material);

                PopulateBiomeWithObjects(biomePtr, prefix);

                m_Biomes.emplace_back(std::move(biomePtr));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Terrain::ComputeMaps() {
        vd::img::IMGLoader imgLoader;
        m_HeightImg = imgLoader.loadFloatImage(m_PropsPtr->Get<std::string>("HeightMap"));
        m_HeightMap = std::make_shared<vd::model::Texture2D>(m_HeightImg);

        normalmap::NormalMapRendererPtr normalMapRendererPtr =
                std::make_shared<normalmap::NormalMapRenderer>(int(m_HeightMap->getWidth()));
        normalMapRendererPtr->render(m_HeightMap, m_PropsPtr->Get<float>("NormalStrength"));
        m_NormalMap = normalMapRendererPtr->getNormalMap();

        splatmap::SplatMapRendererPtr splatMapRendererPtr =
                std::make_shared<splatmap::SplatMapRenderer>(int(m_HeightMap->getWidth()));
        splatMapRendererPtr->render(m_HeightMap, m_PropsPtr->Get<float>("ScaleY"), m_Biomes);
        m_SplatMap = splatMapRendererPtr->getSplatMap();
        m_SplatImg = splatMapRendererPtr->getSplatData();
    }

    void Terrain::PopulateBiomeWithObjects(BiomePtr& biomePtr, const std::string& biomePrefix) {
        for (int i = 0; ; ++i) {
            try {
               const std::string objectPrefix = biomePrefix + ".Object." + std::to_string(i);

               const auto kLocation = m_PropsPtr->Get<std::string>(objectPrefix + ".Location");
               const auto kFile = m_PropsPtr->Get<std::string>(objectPrefix + ".File");
               const auto kScale = m_PropsPtr->Get<float>(objectPrefix + ".Scale");

                sobj::StaticObjectPtr staticObjectPtr = std::make_shared<sobj::StaticObject>(kLocation, kFile);
                staticObjectPtr->GetWorldTransform().SetScaling(kScale, kScale, kScale);
                staticObjectPtr->Init();

                biomePtr->addObject(staticObjectPtr);
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Terrain::GeneratePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        auto& vertices = meshPtr->vertices;
        vertices.resize(16);

        vertices[0] = { .Position = glm::vec3(0.0f, 0.0f, 0.0f) };
        vertices[1] = { .Position = glm::vec3(0.333f, 0.0f, 0.0f) };
        vertices[2] = { .Position = glm::vec3(0.666f, 0.0f, 0.0f) };
        vertices[3] = { .Position = glm::vec3(1.0f, 0.0f, 0.0f) };

        vertices[4] = { .Position = glm::vec3(0.0f, 0.0f, 0.333f) };
        vertices[5] = { .Position = glm::vec3(0.333f, 0.0f, 0.333f) };
        vertices[6] = { .Position = glm::vec3(0.666f, 0.0f, 0.333f) };
        vertices[7] = { .Position = glm::vec3(1.0f, 0.0f, 0.333f) };

        vertices[8] = { .Position = glm::vec3(0.0f, 0.0f, 0.666f) };
        vertices[9] = { .Position = glm::vec3(0.333f, 0.0f, 0.666f) };
        vertices[10] = { .Position = glm::vec3(0.666f, 0.0f, 0.666f) };
        vertices[11] = { .Position = glm::vec3(1.0f, 0.0f, 0.666f) };

        vertices[12] = { .Position = glm::vec3(0.0f, 0.0f, 1.0f) };
        vertices[13] = { .Position = glm::vec3(0.333f, 0.0f, 1.0f) };
        vertices[14] = { .Position = glm::vec3(0.666f, 0.0f, 1.0f) };
        vertices[15] = { .Position = glm::vec3(1.0f, 0.0f, 1.0f) };

        SetBufferGenerationStrategy(ePatch);

        GetMeshes().push_back(meshPtr);
    }

    void Terrain::PopulateTree(const TerrainNode::ptr_type_t& root) {
        const auto kLevelOffset = m_PropsPtr->Get<int>("LevelOffset");

        if (root->GetLevel() < kLevelOffset) {
            root->Populate();

            for (auto& child : root->GetChildren()) {
                auto terrainNodeChild = std::dynamic_pointer_cast<TerrainNode>(child);
                PopulateTree(terrainNodeChild);
            }
        } else {
            m_ImaginaryRootNodes.emplace_back(std::dynamic_pointer_cast<TerrainNode>(root));
            m_ImaginaryRootNodes.back()->ResetLevel();
        }
    }
}