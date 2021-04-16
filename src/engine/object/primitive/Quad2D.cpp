//
// Created by Vali on 2/26/2021.
//

#include "Quad2D.hpp"

namespace vd::object::primitive {
    void Quad2D::Setup() { }

    void Quad2D::Init() {
        vd::model::MeshPtr& pMesh = Meshes()
                .emplace_back(
                    std::make_shared<vd::model::Mesh>(
                        vd::gapi::AttributeTypeVec({
                            vd::gapi::AttributeType::FLOAT_2
                        })
                    )
                );

        pMesh->Assign(vd::gapi::DataFragmentation::eAsTriangleStrip,
                      {
                            vd::model::Vertex(std::vector<float>({ -1.0f, 1.0f })),
                            vd::model::Vertex(std::vector<float>({ -1.0f, -1.0f })),
                            vd::model::Vertex(std::vector<float>({ 1.0f, 1.0f })),
                            vd::model::Vertex(std::vector<float>({ 1.0f, -1.0f }))
                      });

        /*vd::gl::wrappers::BufferPtr& pBuffer = Buffers().emplace_back(std::move(std::make_shared<vd::gl::wrappers::Buffer>()));
        pBuffer->Create();
        pBuffer->Bind();
        pBuffer->AddBuffer(
                gl::eArrayBuffer,
                pMesh->VerticesSizeInBytes(),
                &pMesh->Data()[0],
                gl::eStaticDraw
        );
        pBuffer->AttributeArray(0, 0, 2, vd::gl::eFloat, pMesh->VertexSizeInBytes(), (GLvoid*)0);
        pBuffer->AttributeArray(1, 0, 2, vd::gl::eFloat, pMesh->VertexSizeInBytes(), (GLvoid*)(2 * 4));
        pBuffer->Unbind();

        BoundingBoxes().emplace_back();
        BoundingBoxes().back().WrapMesh(pMesh);*/
    }

    void Quad2D::Update() {

    }
}

