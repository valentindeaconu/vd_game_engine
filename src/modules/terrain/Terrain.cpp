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

        m_MapSize = scaleXZ;
        m_ScaleY = scaleY;

        // Compute radius & center
        ComputeCenterAndRadius();

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

        Entity2D::CleanUp();
    }

    float Terrain::MapSize() const {
        return m_MapSize;
    }

    glm::vec2 Terrain::Radius() const {
        return m_Radius;
    }

    const glm::vec2& Terrain::Center() const {
        return m_Center;
    }

    const vd::property::PropertiesPtr& Terrain::Properties() const {
        return m_pProperties;
    }

    const std::vector<TerrainNode::ptr_type_t>& Terrain::RootNodes() const {
        return m_ImaginaryRootNodes;
    }

    const vd::gl::Texture2DPtr& Terrain::HeightMap() const {
        return m_pHeightMap;
    }

    const vd::gl::Texture2DPtr& Terrain::NormalMap() const {
        return m_pNormalMap;
    }

    glm::vec2 Terrain::ToTerrainUV(float x, float z) const {
        const glm::vec3 T = const_cast<Terrain*>(this)->WorldTransform().Translation();
        const auto invT = -T;

        const float lowerBound = T.x;
        const float upperBound = lowerBound + m_MapSize;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return glm::vec2(-1, -1);

        // reverse transform
        glm::vec3 coords = (glm::vec3(x, 0, z) + invT) / m_MapSize;
        
        return glm::vec2(coords.x, coords.z);
    }

    float Terrain::HeightAt(float x, float z) const {
        const auto uv = ToTerrainUV(z, x);

        if (uv.x < 0.0f || uv.y < 0.0f) {
            return 0.0f;
        }

        const auto height = m_pHeightImg->Get<float, vd::math::Interpolation::eBilinear>(uv);

        return height * m_ScaleY;
    }

    void Terrain::ComputeCenterAndRadius() {
        const auto lowerPerc = m_pProperties->Get<float>("Fade.LowerPercentage");
        const auto upperPerc = m_pProperties->Get<float>("Fade.UpperPercentage");

        const auto halfUnit = m_MapSize * 0.5f;

        m_Radius.y = upperPerc * halfUnit;
        m_Radius.x = lowerPerc * halfUnit;

        glm::vec3 center = WorldTransform().Translation() + glm::vec3(m_MapSize * .5f, 0.0f, m_MapSize * .5f);
        m_Center = glm::vec2(center.x, center.z);
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