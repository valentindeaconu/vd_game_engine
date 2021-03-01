#include "Sky.hpp"

namespace mod::sky {

    void Sky::Setup() {
        vd::model::Mesh3DPtr pMesh = std::make_shared<vd::model::Mesh3D>();

        for (int i = 0; i < 24; i += 3) {
            pMesh->Vertices().emplace_back(
                    glm::vec3(kSkyboxVertices[i], kSkyboxVertices[i + 1], kSkyboxVertices[i + 2]),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec2(kSkyboxVertices[i], kSkyboxVertices[i + 1])
            );
        }

        pMesh->Indices() = kSkyboxIndices;

        this->PushMesh({ pMesh }, 1000.0f);
    }

    void Sky::Update() {

    }

}