#include "Sky.hpp"

namespace mod::sky {

    Sky::Sky() = default;
    Sky::~Sky() = default;

    void Sky::Init() {
        vd::model::MeshPtrVec& meshPtrVec = Meshes();

        vd::model::MeshPtr pMesh = std::make_shared<vd::model::Mesh>();

        for (int i = 0; i < 24; i += 3) {
            pMesh->Vertices().emplace_back(
                glm::vec3(kSkyboxVertices[i], kSkyboxVertices[i + 1], kSkyboxVertices[i + 2]),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec2(kSkyboxVertices[i], kSkyboxVertices[i + 1])
            );
        }

        pMesh->Indices() = kSkyboxIndices;

        meshPtrVec.push_back(pMesh);

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void Sky::Update() {
    }

    void Sky::CleanUp() {
        Entity::CleanUp(); // call super.CleanUp() to clear meshBuffers;
    }
}