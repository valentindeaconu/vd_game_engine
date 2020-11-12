#include "Sky.hpp"

namespace mod::sky {

    Sky::Sky() = default;
    Sky::~Sky() = default;

    void Sky::Init() {
        vd::model::MeshPtrVec& meshPtrVec = Meshes();

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

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void Sky::Update() {
    }

    void Sky::CleanUp() {
        Entity::CleanUp(); // call super.CleanUp() to clear meshBuffers;
    }
}