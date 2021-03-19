//
// Created by Vali on 9/21/2020.
//

#include "Terrain.hpp"

namespace mod::terrain {

    Terrain::Terrain(const std::string& propsFilePath)
        : m_pProperties(vd::loader::PropertiesLoader::Load(propsFilePath))
    {
    }

    void Terrain::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
    }

    void Terrain::Setup() { }

    void Terrain::Init() {
        CreateProps();

        // Create world transform
        const auto scaleXZ = m_pProperties->Get<float>("ScaleXZ");
        const auto scaleY = m_pProperties->Get<float>("ScaleY");
        WorldTransform().Scale() = glm::vec3(scaleXZ, scaleY, scaleXZ);
        WorldTransform().Translation() = glm::vec3(-scaleXZ / 2.0f, 0.0f, -scaleXZ / 2.0f);

        PopulateBiomes();

        ComputeMaps();

        auto convertToWorldCoords = [i = m_pHeightImg, t = WorldTransform().Get()](float x, float y) {
            const auto h = i->Get<float, vd::math::Interpolation::eBilinear>(glm::vec2(x, y));
            return glm::vec3(t * glm::vec4(x, h, y, 1.0f));
        };

        m_RootNode = std::make_shared<TerrainNode>(nullptr,
                                                   glm::vec2(0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f),
                                                   convertToWorldCoords,
                                                   m_pProperties->Get<int>("MaxLevelOfDetail"),
                                                   &m_LevelOfDetailRanges,
                                                   0,
                                                   TerrainNode::eRootNode);

        PopulateTree(m_RootNode);

        std::vector<glm::vec2> vertices = GeneratePatch();

        vd::gl::BufferPtr& pBuffer = Buffers().emplace_back(std::move(std::make_shared<vd::gl::Buffer>()));

        pBuffer->Create();
        pBuffer->Bind();

        pBuffer->AddBuffer(
                vd::gl::eArrayBuffer,
                vertices.size() * sizeof(glm::vec2),
                &vertices[0],
                vd::gl::eStaticDraw
        );

        pBuffer->AttributeArray(0, 2, vd::gl::eFloat, sizeof(glm::vec2), (GLvoid*)0);

        pBuffer->PatchParameter(vd::gl::ePatchVertices, vertices.size());

        pBuffer->Unbind();
    }

    void Terrain::Update() {
        if (m_pProperties->Get<bool>("LevelOfDetailEnabled")) {
            if (m_pCamera->CameraMoved() || m_pCamera->CameraRotated()) {
                for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                    imaginaryRootNode->Update(m_pCamera);
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

        for (auto& biome : m_Biomes) {
            for (auto& prop : biome->Props()) {
                prop->CleanUp();
            }
        }

        Entity2D::CleanUp();
    }

    const vd::property::PropertiesPtr& Terrain::Properties() const {
        return m_pProperties;
    }

    const std::vector<TerrainNode::ptr_type_t>& Terrain::RootNodes() const {
        return m_ImaginaryRootNodes;
    }

    const BiomePtrVec& Terrain::Biomes() const {
        return m_Biomes;
    }

    const vd::gl::Texture2DPtr& Terrain::HeightMap() const {
        return m_pHeightMap;
    }

    const vd::gl::Texture2DPtr& Terrain::NormalMap() const {
        return m_pNormalMap;
    }

    const vd::gl::Texture2DPtr& Terrain::SplatMap() const {
        return m_pSplatMap;
    }

    float Terrain::HeightAt(float x, float z) const {
        const auto scaleXZ = m_pProperties->Get<float>("ScaleXZ");
        const auto scaleY = m_pProperties->Get<float>("ScaleY");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return 0.0f;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto height = m_pHeightImg->Get<float, vd::math::Interpolation::eBilinear>(glm::vec2(rz, rx));

        return height * scaleY;
    }

    BiomePtrVec Terrain::BiomesAt(float x, float z) const {
        const auto scaleXZ = m_pProperties->Get<float>("ScaleXZ");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        BiomePtrVec output;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return output;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto mask = m_pSplatImg->Get<uint32_t, vd::math::Interpolation::eNearestNeighbour>(glm::vec2(rz, rx));

        for (size_t k = 0; k < m_Biomes.size(); ++k) {
            if ((mask & (1 << k)) != 0) {
                output.emplace_back(m_Biomes[k]);
            }
        }

        return output;
    }

    void Terrain::CreateProps() {
        // Level offset (Jump over imaginary root nodes)
        const auto kRootNodes = m_pProperties->Get<int>("RootNodes");
        const int kLevelOffset = int(std::log(kRootNodes) / std::log(4));
        if (!m_pProperties->Set("LevelOffset", std::to_string(kLevelOffset))) {
            vd::Logger::terminate("Could not add level offset to the properties", 1);
        }
        // Max Level of detail
        for (int i = 0; ; ++i) {
            try {
                m_LevelOfDetailRanges.emplace_back(m_pProperties->Get<int>("Lod." + std::to_string(i) + ".Range"));
            } catch (std::invalid_argument& e) {
                if (!m_pProperties->Set("MaxLevelOfDetail", std::to_string(i))) {
                    vd::Logger::terminate("Could not add max level of detail to the properties", 1);
                }
                break;
            }
        }
    }

    void Terrain::PopulateBiomes() {
        const auto scaleY = m_pProperties->Get<float>("ScaleY");
        for (int i = 0; ; ++i) {
            try {
                const std::string prefix = "Biome." + std::to_string(i);

                BiomePtr biomePtr = std::make_shared<Biome>();

                biomePtr->Name() = m_pProperties->Get<std::string>(prefix + ".Name");
                biomePtr->MinimumHeight() = m_pProperties->Get<float>(prefix + ".MinHeight") * scaleY;
                biomePtr->MaximumHeight() = m_pProperties->Get<float>(prefix + ".MaxHeight") * scaleY;

                const std::string materialPrefix = prefix + ".Material";
                biomePtr->Material().DiffuseMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Diffuse"));
                biomePtr->Material().DiffuseMap()->Bind();
                biomePtr->Material().DiffuseMap()->MipmapLinearFilter();
                biomePtr->Material().DiffuseMap()->Unbind();

                biomePtr->Material().NormalMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Normal"));
                biomePtr->Material().NormalMap()->Bind();
                biomePtr->Material().NormalMap()->MipmapLinearFilter();
                biomePtr->Material().NormalMap()->Unbind();

                biomePtr->Material().DisplaceMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(materialPrefix + ".Displace"));
                biomePtr->Material().DisplaceMap()->Bind();
                biomePtr->Material().DisplaceMap()->MipmapLinearFilter();
                biomePtr->Material().DisplaceMap()->Unbind();

                biomePtr->Material().DisplaceScale() = m_pProperties->Get<float>(materialPrefix + ".HeightScaling");
                biomePtr->Material().HorizontalScale() = m_pProperties->Get<float>(materialPrefix + ".HorizontalScaling");

                PopulateBiomeWithProps(biomePtr, prefix);

                m_Biomes.emplace_back(std::move(biomePtr));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Terrain::ComputeMaps() {
        const auto kHeightMapPath = m_pProperties->Get<std::string>("HeightMap");
        m_pHeightImg = vd::loader::ImageLoader::Load<float, vd::model::ImageFormat::eR>(kHeightMapPath);

        m_pHeightMap = vd::service::TextureService::Create(
                kHeightMapPath,
                m_pHeightImg->Dimension(),
                vd::gl::TextureFormat::eR8,
                vd::gl::TextureFormat::eR,
                vd::gl::DataType::eFloat,
                &m_pHeightImg->Data()[0]
        );

        m_pHeightMap->Bind();
        m_pHeightMap->LinearFilter();
        m_pHeightMap->Unbind();

        const int size = int(m_pHeightMap->Width());
        const auto strength = m_pProperties->Get<float>("NormalStrength");
        const auto scaleY = m_pProperties->Get<float>("ScaleY");

        normalmap::NormalMapBuilder NMBuilder;
        NMBuilder.Create(m_pHeightMap, size, strength, m_pNormalMap);
        NMBuilder.CleanUp();

        splatmap::SplatMapBuilder SMBuilder;
        SMBuilder.Create(m_pHeightMap, size, scaleY, m_Biomes, m_pSplatMap, m_pSplatImg);
        SMBuilder.CleanUp();
    }

    void Terrain::PopulateBiomeWithProps(BiomePtr& pBiome, const std::string& biomePrefix) {
        for (int i = 0; ; ++i) {
            try {
               const std::string objectPrefix = biomePrefix + ".Object." + std::to_string(i);

               const auto kScale = m_pProperties->Get<float>(objectPrefix + ".Scale");

               std::vector<props::Prop::Details> propsDetails;
               for (int j = 0; ; ++j) {
                   try {
                       const std::string lodObjectPrefix = objectPrefix + ".Level." + std::to_string(j);

                       props::Prop::Details propDetails;
                       propDetails.Path = m_pProperties->Get<std::string>(lodObjectPrefix + ".Location");
                       propDetails.File = m_pProperties->Get<std::string>(lodObjectPrefix + ".File");
                       propDetails.Distance = m_pProperties->Get<float>(lodObjectPrefix + ".Distance");
                       propDetails.Billboard = m_pProperties->Get<bool>(lodObjectPrefix + ".Billboard");

                       propsDetails.emplace_back(std::move(propDetails));
                   } catch (std::invalid_argument& e) {
                       break;
                   }
               }

                props::PropPtr pProp = std::make_shared<props::Prop>(propsDetails);
                pProp->WorldTransform().Scale() = glm::vec3(kScale, kScale, kScale);
                pProp->Init();

                pBiome->Props().emplace_back(std::move(pProp));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    std::vector<glm::vec2> Terrain::GeneratePatch() {
        std::vector<glm::vec2> vertices(16);

        vertices[0] = glm::vec2(0.0f, 0.0f);
        vertices[1] = glm::vec2(0.333f, 0.0f);
        vertices[2] = glm::vec2(0.666f, 0.0f);
        vertices[3] = glm::vec2(1.0f, 0.0f);

        vertices[4] = glm::vec2(0.0f, 0.333f);
        vertices[5] = glm::vec2(0.333f,  0.333f);
        vertices[6] = glm::vec2(0.666f, 0.333f);
        vertices[7] = glm::vec2(1.0f, 0.333f);

        vertices[8] = glm::vec2(0.0f, 0.666f);
        vertices[9] = glm::vec2(0.333f, 0.666f);
        vertices[10] = glm::vec2(0.666f, 0.666f);
        vertices[11] = glm::vec2(1.0f, 0.666f);

        vertices[12] = glm::vec2(0.0f, 1.0f);
        vertices[13] = glm::vec2(0.333f, 1.0f);
        vertices[14] = glm::vec2(0.666f, 1.0f);
        vertices[15] = glm::vec2(1.0f, 1.0f);

        return vertices;
    }

    void Terrain::PopulateTree(const TerrainNode::ptr_type_t& root) {
        const auto kLevelOffset = m_pProperties->Get<int>("LevelOffset");

        if (root->Level() < kLevelOffset) {
            root->Populate();

            for (auto& child : root->Children()) {
                auto terrainNodeChild = std::dynamic_pointer_cast<TerrainNode>(child);
                PopulateTree(terrainNodeChild);
            }
        } else {
            m_ImaginaryRootNodes.emplace_back(std::dynamic_pointer_cast<TerrainNode>(root));
            m_ImaginaryRootNodes.back()->ResetLevel();
        }
    }
}