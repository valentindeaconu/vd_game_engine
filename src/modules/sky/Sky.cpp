#include "Sky.hpp"

namespace mod::sky
{
    Sky::Sky(const vd::EnginePtr& enginePtr)
        : Entity(enginePtr)
    {
    }

    Sky::~Sky() = default;

    void Sky::init()
    {
        vd::model::MeshPtrVec& meshPtrVec = getMeshes();

        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        for (int i = 0; i < 24; i += 3) {
            meshPtr->vertices.emplace_back();
            vd::model::Vertex& vertex = meshPtr->vertices.back();

            vertex.Position = glm::vec3(kSkyboxVertices[i], kSkyboxVertices[i + 1], kSkyboxVertices[i + 2]);
            vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.TexCoords = glm::vec2(kSkyboxVertices[i], kSkyboxVertices[i + 1]);
        }

        meshPtr->indices = kSkyboxIndices;

        meshPtrVec.push_back(meshPtr);

        Entity::init(); // call super.init() to initialize meshBuffers;
    }

    void Sky::update()
    {
        // here should be the input handler for player movement
    }

    void Sky::cleanUp()
    {
        Entity::cleanUp(); // call super.cleanUp() to clear meshBuffers;
    }
}