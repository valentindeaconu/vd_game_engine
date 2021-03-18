//
// Created by Vali on 2/26/2021.
//

#include "Quad2D.hpp"

namespace vd::object::primitive {
    void Quad2D::Setup() { }

    void Quad2D::Init() {
        vd::model::Mesh2DPtr& pMesh = Meshes().emplace_back(std::move(std::make_shared<vd::model::Mesh2D>()));
        pMesh->Vertices() = {
                vd::model::Vertex2D(-1.0f, 1.0f),
                vd::model::Vertex2D(-1.0f, -1.0f),
                vd::model::Vertex2D(1.0f, 1.0f),
                vd::model::Vertex2D(1.0f, -1.0f)
        };

        vd::gl::BufferPtr& pBuffer = Buffers().emplace_back(std::move(std::make_shared<vd::gl::Buffer>()));
        pBuffer->Create();
        pBuffer->Bind();
        pBuffer->AddBuffer(
                gl::eArrayBuffer,
                pMesh->Vertices().size() * sizeof(vd::model::Vertex2D),
                &pMesh->Vertices()[0],
                gl::eStaticDraw
        );
        pBuffer->AttributeArray(0, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)0);
        pBuffer->AttributeArray(1, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)offsetof(vd::model::Vertex2D, TexCoords));
        pBuffer->Unbind();

        BoundingBoxes().emplace_back();
        BoundingBoxes().back().WrapMesh(pMesh);
    }

    void Quad2D::Update() {

    }
}

