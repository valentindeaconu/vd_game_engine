//
// Created by Vali on 2/26/2021.
//

#include "Quad2D.hpp"

namespace vd::object::primitive {
    void Quad2D::Setup() {
        vd::model::Mesh2DPtr pMesh = std::make_shared<vd::model::Mesh2D>();

        pMesh->Vertices() = {
                vd::model::Vertex2D(-1.0f, 1.0f),
                vd::model::Vertex2D(-1.0f, -1.0f),
                vd::model::Vertex2D(1.0f, 1.0f),
                vd::model::Vertex2D(1.0f, -1.0f)
        };

        pMesh->Indices() = {0, 1, 2, 2, 1, 3};

        Meshes().emplace_back(std::move(pMesh));
    }

    void Quad2D::Update() {

    }
}

