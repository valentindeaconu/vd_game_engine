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

        auto rootNodes = m_ConfigPtr->getRootNodes();
        auto sqrtRootNodes = int(std::floor(std::sqrt(rootNodes)));

        m_RootNodes.reserve(sqrtRootNodes * sqrtRootNodes);

        for (int i = 0; i < sqrtRootNodes; ++i) {
            for (int j = 0; j < sqrtRootNodes; ++j) {
                const auto x = float(i);
                const auto y = float(j);
                const auto gridFactor = float(sqrtRootNodes);

                const glm::vec2 topLeft(x / gridFactor, y / gridFactor);
                const glm::vec2 bottomRight((x + 1) / gridFactor, (y + 1) / gridFactor);

                TerrainNodePtr nodePtr = std::make_shared<TerrainNode>(nullptr,
                                                                       m_ConfigPtr,
                                                                       topLeft,
                                                                       bottomRight,
                                                                       0,
                                                                       TerrainNode::eRootNode);

                if (i == 0) {
                    nodePtr->SetNeighbour(nullptr, TerrainNode::eTop);
                } else {
                    const auto &topNeighbour = m_RootNodes[(i - 1) * sqrtRootNodes + j];
                    nodePtr->SetNeighbour(topNeighbour, TerrainNode::eTop);

                    topNeighbour->SetNeighbour(nodePtr, TerrainNode::eBottom);
                }

                if (j == 0) {
                    nodePtr->SetNeighbour(nullptr, TerrainNode::eLeft);
                } else {
                    const auto &leftNeighbour = m_RootNodes[i * sqrtRootNodes + (j - 1)];
                    nodePtr->SetNeighbour(leftNeighbour, TerrainNode::eLeft);

                    leftNeighbour->SetNeighbour(nodePtr, TerrainNode::eRight);
                }

                nodePtr->SetNeighbour(nullptr, TerrainNode::eRight);
                nodePtr->SetNeighbour(nullptr, TerrainNode::eBottom);

                m_RootNodes.push_back(std::move(nodePtr));
            }
        }

        generatePatch();

        Entity::init();
    }

    void Terrain::update() {
        auto& cameraPtr = getParentEngine()->getCamera();
        if (cameraPtr->isCameraMoved() || cameraPtr->isCameraRotated()) {
            for (auto& rootNode : m_RootNodes) {
                rootNode->Update(cameraPtr);
            }

            for (auto& rootNode : m_RootNodes) {
                rootNode->UpdateNeighbours();
            }
        }
    }

    void Terrain::cleanUp() {
        m_RootNodes.clear();

        Entity::cleanUp();
    }

    const TerrainConfigPtr& Terrain::GetTerrainConfig() const {
        return m_ConfigPtr;
    }

    const TerrainNodePtrVec& Terrain::GetRootNodes() const {
        return m_RootNodes;
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
}