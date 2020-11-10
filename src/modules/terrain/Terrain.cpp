//
// Created by Vali on 9/21/2020.
//

#include "Terrain.hpp"

namespace mod::terrain {

    Terrain::Terrain(const vd::EnginePtr& enginePtr, const std::string& configFilePath)
        : vd::object::Entity(enginePtr)
        , m_ConfigPtr(std::make_shared<TerrainConfig>(configFilePath))
    {
    }

    Terrain::~Terrain() = default;

    void Terrain::init() {
        m_ConfigPtr->parse();

        auto& biomes = m_ConfigPtr->getBiomes();
        for (const auto& biome : biomes) {
            auto& objects = biome->getObjects();
            for (auto& object : objects) {
                object->setParentEngine(getParentEngine());
            }
        }

        m_RootNode = std::make_shared<TerrainNode>(nullptr,
                                                   m_ConfigPtr,
                                                   glm::vec2(0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f),
                                                   0,
                                                   TerrainNode::eRootNode);

        populateTree(m_RootNode);

        generatePatch();

        Entity::init();
    }

    void Terrain::update() {
        auto& cameraPtr = getParentEngine()->getCamera();
        if (cameraPtr->isCameraMoved() || cameraPtr->isCameraRotated()) {
            //m_RootNode->Update(cameraPtr);
            //m_RootNode->UpdateNeighbours();
            for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                imaginaryRootNode->Update(cameraPtr);
            }

            for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                imaginaryRootNode->UpdateNeighbours();
            }
        }
    }

    void Terrain::cleanUp() {
        m_RootNode->Clear();
        m_ImaginaryRootNodes.clear();

        m_RootNode = nullptr;

        Entity::cleanUp();
    }

    const TerrainConfigPtr& Terrain::GetTerrainConfig() const {
        return m_ConfigPtr;
    }

    void Terrain::generatePatch() {
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

        setBufferGenerationStrategy(ePatch);

        getMeshes().push_back(meshPtr);
    }

    const std::vector<TerrainNode::ptr_type_t>& Terrain::GetRootNodes() const {
        return m_ImaginaryRootNodes;
    }

    void Terrain::populateTree(const TerrainNode::ptr_type_t& root) {
        // TODO: Set level offset
        if (root->GetLevel() < 3) {
            root->Populate();

            for (auto& child : root->GetChildren()) {
                auto terrainNodeChild = std::dynamic_pointer_cast<TerrainNode>(child);
                populateTree(terrainNodeChild);
            }
        } else {
            m_ImaginaryRootNodes.emplace_back(std::dynamic_pointer_cast<TerrainNode>(root));
            m_ImaginaryRootNodes.back()->ResetLevel();
        }
    }
}