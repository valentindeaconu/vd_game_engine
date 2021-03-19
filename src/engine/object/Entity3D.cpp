//
// Created by Vali on 2/17/2021.
//

#include "Entity3D.hpp"

namespace vd::object {

    Entity3D::Entity3D()
        : m_Count(0)
    {
    }

    void Entity3D::Init() {
        Setup();

        if (m_Count == 0) {
            throw RuntimeError("could not initialise a 3D entity without meshes");
        }

        Buffers().clear();
        for (int i = 0; i < m_Count; ++i) {
            auto& meshes = m_DetailedMeshes.Meshes[i];

            auto& bufferIndices = m_DetailedMeshes.BufferIndices[i];
            auto& boundingBoxes = m_DetailedMeshes.BoundingBoxes[i];

            bufferIndices.clear();
            bufferIndices.reserve(meshes.size());

            boundingBoxes.clear();
            boundingBoxes.reserve(meshes.size());

            for (auto& mesh : meshes) {
                bufferIndices.emplace_back(Buffers().size());

                gl::BufferPtr pBuffer = std::make_shared<vd::gl::Buffer>();
                pBuffer->Create();
                pBuffer->Bind();
                pBuffer->AddBuffer(
                        gl::eArrayBuffer,
                        mesh->Vertices().size() * sizeof(vd::model::Vertex3D),
                        &mesh->Vertices()[0],
                        gl::eStaticDraw
                );
                pBuffer->AddBuffer(
                        gl::eElementArrayBuffer,
                        mesh->Indices().size() * sizeof(GLuint),
                        &mesh->Indices()[0],
                        gl::eStaticDraw
                );
                pBuffer->AttributeArray(0, 3, vd::gl::eFloat, sizeof(vd::model::Vertex3D), (GLvoid*)0);
                pBuffer->AttributeArray(1, 3, vd::gl::eFloat, sizeof(vd::model::Vertex3D), (GLvoid*)offsetof(vd::model::Vertex3D, Normal));
                pBuffer->AttributeArray(2, 2, vd::gl::eFloat, sizeof(vd::model::Vertex3D), (GLvoid*)offsetof(vd::model::Vertex3D, TexCoords));
                pBuffer->Unbind();

                Buffers().emplace_back(std::move(pBuffer));

                boundingBoxes.emplace_back(math::Bounds3(mesh));
            }
        }
    }

    void Entity3D::CleanUp() {
        for (auto& b : Buffers()) {
            b->CleanUp();
        }
        Buffers().clear();

        m_DetailedMeshes.Distances.clear();
        m_DetailedMeshes.Meshes.clear();
        m_DetailedMeshes.BoundingBoxes.clear();
        m_DetailedMeshes.BufferIndices.clear();

    }

    uint64_t Entity3D::LevelOfDetailAtDistance(float distance) {
        for (int i = 0; i < m_Count; ++i) {
            if (distance <= m_DetailedMeshes.Distances[i]) {
                return i;
            }
        }

        return m_Count - 1;
    }

    const model::Mesh3DPtrVec& Entity3D::Meshes(uint64_t index) const {
        return m_DetailedMeshes.Meshes[index];
    }

    const math::Bounds3Vec& Entity3D::BoundingBoxes(uint64_t index) const {
        return m_DetailedMeshes.BoundingBoxes[index];
    }

    const std::vector<uint64_t>& Entity3D::BufferIndices(uint64_t index) const {
        return m_DetailedMeshes.BufferIndices[index];
    }


    void Entity3D::PushMesh(const model::Mesh3DPtrVec& meshes, float distance) {
        m_DetailedMeshes.Distances.emplace_back(distance);
        m_DetailedMeshes.Meshes.emplace_back(meshes);

        /// Allocate space for buffer indices and bounding boxes
        m_DetailedMeshes.BufferIndices.emplace_back();
        m_DetailedMeshes.BoundingBoxes.emplace_back();

        m_Count++;
    }

    void Entity3D::UpdateBoundsForLevel(uint64_t levelOfDetail) {
        const auto sz = m_DetailedMeshes.Meshes[levelOfDetail].size();
        m_DetailedMeshes.BoundingBoxes[levelOfDetail].clear();
        m_DetailedMeshes.BoundingBoxes[levelOfDetail].resize(sz);

        for (int i = 0; i < sz; ++i) {
            math::Bounds3 bounds(m_DetailedMeshes.Meshes[levelOfDetail][i]);
            m_DetailedMeshes.BoundingBoxes[levelOfDetail][i] = bounds.WithTransform(WorldTransform());
        }
    }
}