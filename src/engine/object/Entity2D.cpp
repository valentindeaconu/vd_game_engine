//
// Created by Vali on 2/17/2021.
//

#include "Entity2D.hpp"

namespace vd::object {
    void Entity2D::Init() {
        Setup();

        if (m_Meshes.empty()) {
            throw RuntimeError("could not initialise a 2D entity without meshes");
        }

        /*for (auto& mesh : m_Meshes) {
            Buffers().emplace_back(std::move(std::make_shared<vd::gl::Buffer>()));
            vd::gl::BufferPtr pBuffer = Buffers().back();

            pBuffer->Create();
            pBuffer->Bind();

            pBuffer->AddBuffer(
                    gl::eArrayBuffer,
                    mesh->VerticesSizeInBytes(),
                    &mesh->Data()[0],
                    gl::eStaticDraw
            );

            pBuffer->AddBuffer(
                    gl::eElementArrayBuffer,
                    mesh->IndicesSizeInBytes(),
                    &mesh->Indices()[0],
                    gl::eStaticDraw
            );

            pBuffer->AttributeArray(0, 0, 2, vd::gl::eFloat, mesh->VertexSizeInBytes(), (GLvoid*)0);
            pBuffer->AttributeArray(1, 0, 2, vd::gl::eFloat, mesh->VertexSizeInBytes(), (GLvoid*)(2 * 4)); // 2 floats * 4 bytes each

            pBuffer->Unbind();

            m_BoundingBoxes.emplace_back();
            m_BoundingBoxes.back().WrapMesh(mesh);
        }*/
    }

    void Entity2D::CleanUp() {
        for (auto& b : Buffers()) {
            b->CleanUp();
        }

        m_Meshes.clear();
        m_BoundingBoxes.clear();
    }

    model::MeshPtrVec& Entity2D::Meshes() {
        return m_Meshes;
    }

    math::Bounds2Vec& Entity2D::BoundingBoxes() {
        return m_BoundingBoxes;
    }
}