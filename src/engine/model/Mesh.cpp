//
// Created by Vali on 12/1/2020.
//

#include "Mesh.hpp"


namespace vd::model {

    Mesh::Mesh(gapi::AttributeTypeVec vertexAttributes)
        : m_VertexAttributes(std::move(vertexAttributes))
        , m_DynamicData(false)
    {
    }

    bool Mesh::Assign(const gapi::DataFragmentation& dataFragmentation,
                      const std::vector<Vertex>& vertices,
                      const std::vector<uint32_t>& indices,
                      bool computeBounds) {

        uint64_t vertexDataSize = gapi::AttributeType::ComputeDataSize(m_VertexAttributes, m_VertexAttributes.size());

        // Extract vertex data
        std::vector<float> data;
        data.reserve(vertexDataSize * vertices.size());

        uint64_t vertexCount = 0;

        for (auto& v : vertices) {
            if (v.SameAttributes({})) {
                // vertex attributes are not set, but we check if we can interpret its raw data as our attributes
                if (v.Data().size() != vertexDataSize) {
                    break;
                }
            } else if (!v.SameAttributes(m_VertexAttributes)) {
                break;
            }

            data.insert(data.end(), v.Data().begin(), v.Data().end());
            vertexCount += 1;
        }

        if (vertexCount != vertices.size()) {
            data.clear();
            vertexCount = 0;
            return false;
        }

        // Compute bounds
        if (computeBounds) {
            ComputeBounds(vertices);
        }

        // Allocate buffer
        if (m_Buffer) {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
        m_Buffer = gapi::Factory::CreateBuffer();

        m_Buffer->AllocateStatic(dataFragmentation,
                                 m_VertexAttributes,
                                 gapi::AttributeTypeVec(),
                                 data,
                                 std::vector<float>(),
                                 indices);

        return true;
    }

    bool Mesh::MakeDynamic(const gapi::DataFragmentation& dataFragmentation,
                           uint64_t perVertexMaximumDataSize,
                           uint64_t maximumIndices) {
        m_DynamicData = true;

        // Allocate buffer
        if (m_Buffer) {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
        m_Buffer = gapi::Factory::CreateBuffer();

        m_Buffer->AllocateDynamic(dataFragmentation,
                                  m_VertexAttributes,
                                  gapi::AttributeTypeVec(),
                                  perVertexMaximumDataSize,
                                  maximumIndices,
                                  0);

        return true;
    }

    bool Mesh::DynamicSetVertexData(const std::vector<Vertex>& vertices, bool computeBounds) {
        if (!m_DynamicData)
            return false;

        std::vector<float> data;
        if (!SolveVertexVec(vertices, m_VertexAttributes, data)) {
            return false;
        }

        // Compute bounds
        if (computeBounds) {
            ComputeBounds(vertices);
        }

        m_Buffer->UpdateVertexData(data, vertices.size());

        return true;
    }

    bool Mesh::DynamicSetIndicesData(const std::vector<uint32_t>& indices) {
        m_Buffer->UpdateIndices(indices);
        return true;
    }

    void Mesh::Draw() const {
        m_Buffer->Draw();
    }

    model::Material& Mesh::Material() {
        return m_Material;
    }

    void Mesh::ComputeBounds(const std::vector<Vertex>& vertices) {
        // TODO: Use compute bounds to compute real bounds of the mesh
    }

    bool Mesh::SolveVertexVec(const std::vector<Vertex>& vertices,
                              const gapi::AttributeTypeVec& attributes,
                              std::vector<float>& output) {
        uint64_t vertexDataSize = gapi::AttributeType::ComputeDataSize(attributes, attributes.size());

        output.clear();
        output.reserve(vertexDataSize * vertices.size());

        uint64_t vertexCount = 0;

        for (auto& v : vertices) {
            if (v.SameAttributes({})) {
                // vertex attributes are not set, but we check if we can interpret its raw data as our attributes
                if (v.Data().size() != vertexDataSize) {
                    break;
                }
            } else if (!v.SameAttributes(attributes)) {
                break;
            }

            output.insert(output.end(), v.Data().begin(), v.Data().end());
            vertexCount += 1;
        }

        if (vertexCount != vertices.size()) {
            output.clear();
            return false;
        }

        return true;
    }

    InstanceMesh::InstanceMesh(const gapi::AttributeTypeVec& vertexAttributes,
                               const gapi::AttributeTypeVec& instanceAttributes)
        : m_Mesh(vertexAttributes)
        , m_InstanceAttributes(instanceAttributes)
    {
    }

    bool InstanceMesh::Assign(const gapi::DataFragmentation& dataFragmentation,
                              const std::vector<Vertex>& vertexData,
                              const std::vector<uint32_t>& indices,
                              const std::vector<Vertex>& instanceData,
                              bool computeBounds) {
        std::vector<float> vData;
        if (!Mesh::SolveVertexVec(vertexData, m_Mesh.m_VertexAttributes, vData)) {
            return false;
        }

        std::vector<float> iData;
        if (!Mesh::SolveVertexVec(instanceData, m_InstanceAttributes, iData)) {
            return false;
        }

        if (computeBounds) {
            m_Mesh.ComputeBounds(vertexData);
        }

        // Allocate buffer
        if (m_Mesh.m_Buffer) {
            m_Mesh.m_Buffer->Release();
            m_Mesh.m_Buffer = nullptr;
        }
        m_Mesh.m_Buffer = gapi::Factory::CreateBuffer();

        m_Mesh.m_Buffer->AllocateStatic(dataFragmentation,
                                        m_Mesh.m_VertexAttributes,
                                        m_InstanceAttributes,
                                        vData,
                                        iData,
                                        indices);

        return true;
    }

    bool InstanceMesh::MakeDynamic(const gapi::DataFragmentation& dataFragmentation,
                                   uint64_t perVertexMaximumDataSize,
                                   uint64_t maximumIndices,
                                   uint64_t perInstanceMaximumDataSize) {
        m_Mesh.m_DynamicData = true;

        // Allocate buffer
        if (m_Mesh.m_Buffer) {
            m_Mesh.m_Buffer->Release();
            m_Mesh.m_Buffer = nullptr;
        }
        m_Mesh.m_Buffer = gapi::Factory::CreateBuffer();

        m_Mesh.m_Buffer->AllocateDynamic(dataFragmentation,
                                         m_Mesh.m_VertexAttributes,
                                         m_InstanceAttributes,
                                         perVertexMaximumDataSize,
                                         maximumIndices,
                                         perInstanceMaximumDataSize);

        return true;
    }

    bool InstanceMesh::DynamicSetVertexData(const std::vector<Vertex>& vertexData, bool computeBounds) {
        return m_Mesh.DynamicSetVertexData(vertexData, computeBounds);
    }

    bool InstanceMesh::DynamicSetIndicesData(const std::vector<uint32_t>& indices) {
        return m_Mesh.DynamicSetIndicesData(indices);
    }

    bool InstanceMesh::DynamicSetInstanceData(const std::vector<Vertex>& instanceData) {
        if (!m_Mesh.m_DynamicData)
            return false;

        std::vector<float> data;
        if (!Mesh::SolveVertexVec(instanceData, m_InstanceAttributes, data)) {
            return false;
        }

        m_Mesh.m_Buffer->UpdateInstanceData(data);

        return true;
    }

    void InstanceMesh::Draw() const {
        return m_Mesh.Draw();
    }

    model::Material& InstanceMesh::Material() {
        return m_Mesh.Material();
    }
}
