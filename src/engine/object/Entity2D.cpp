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

        for (auto& mesh : m_Meshes) {
            Buffers().emplace_back(std::move(std::make_shared<vd::gl::Buffer>()));
            vd::gl::BufferPtr pBuffer = Buffers().back();

            pBuffer->Bind();

            pBuffer->AddBuffer(
                    gl::buffer::eArrayBuffer,
                    mesh->Vertices().size() * sizeof(vd::model::Vertex2D),
                    &mesh->Vertices()[0],
                    gl::buffer::eStaticDraw
            );

            pBuffer->AddBuffer(
                    gl::buffer::eElementArrayBuffer,
                    mesh->Indices().size() * sizeof(GLuint),
                    &mesh->Indices()[0],
                    gl::buffer::eStaticDraw
            );

            pBuffer->AttributeArray(0, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)0);
            pBuffer->AttributeArray(1, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)offsetof(vd::model::Vertex2D, TexCoords));

            pBuffer->Unbind();

            m_BoundingBoxes.emplace_back();
            m_BoundingBoxes.back().WrapMesh(mesh);
        }
    }

    void Entity2D::CleanUp() {
        m_Meshes.clear();
        m_BoundingBoxes.clear();
    }

    model::Mesh2DPtrVec& Entity2D::Meshes() {
        return m_Meshes;
    }

    math::Bounds2Vec& Entity2D::BoundingBoxes() {
        return m_BoundingBoxes;
    }
}