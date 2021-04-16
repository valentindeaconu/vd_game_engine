//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_MESH_HPP
#define VD_GAME_ENGINE_MESH_HPP

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Material.hpp"

#include <engine/math/Bounds.hpp>

#include <engine/defines/Macros.hpp>
#include <engine/gapi/Factory.hpp>

namespace vd::model {
    class InstanceMesh;

    class Mesh {
    public:
        explicit Mesh(gapi::AttributeTypeVec  vertexAttributes);

        Mesh(const Mesh&) = default;

        bool Assign(const gapi::DataFragmentation& dataFragmentation,
                    const std::vector<Vertex>& vertices,
                    const std::vector<uint32_t>& indices = std::vector<uint32_t>(),
                    bool computeBounds = true);

        bool MakeDynamic(const gapi::DataFragmentation& dataFragmentation,
                         uint64_t perVertexMaximumDataSize,
                         uint64_t maximumIndices);

        bool DynamicSetVertexData(const std::vector<Vertex>& vertices, bool computeBounds = true);
        bool DynamicSetIndicesData(const std::vector<uint32_t>& indices);

        void Draw() const;

        [[nodiscard]] model::Material& Material();
    private:
        friend class InstanceMesh;

        void ComputeBounds(const std::vector<Vertex>& vertices);
        static bool SolveVertexVec(const std::vector<Vertex>&, const gapi::AttributeTypeVec&, std::vector<float>&);

        bool                    m_DynamicData;
        gapi::AttributeTypeVec  m_VertexAttributes;
        gapi::BufferPtr         m_Buffer;

        model::Material         m_Material;
    };
    typedef std::shared_ptr<Mesh>   MeshPtr;
    typedef std::vector<MeshPtr>    MeshPtrVec;

    class InstanceMesh {
    public:
        InstanceMesh(const gapi::AttributeTypeVec& vertexAttributes,
                     const gapi::AttributeTypeVec& instanceAttributes);

        InstanceMesh(const InstanceMesh&) = default;

        bool Assign(const gapi::DataFragmentation& dataFragmentation,
                    const std::vector<Vertex>& vertexData,
                    const std::vector<uint32_t>& indices,
                    const std::vector<Vertex>& instancesData,
                    bool computeBounds = true);

        bool MakeDynamic(const gapi::DataFragmentation& dataFragmentation,
                         uint64_t perVertexMaximumDataSize,
                         uint64_t maximumIndices,
                         uint64_t perInstanceMaximumDataSize);

        bool DynamicSetVertexData(const std::vector<Vertex>& vertexData, bool computeBounds = true);
        bool DynamicSetIndicesData(const std::vector<uint32_t>& indices);
        bool DynamicSetInstanceData(const std::vector<Vertex>& instanceData);

        void Draw() const;

        [[nodiscard]] model::Material& Material();
    private:
        Mesh                    m_Mesh;
        gapi::AttributeTypeVec  m_InstanceAttributes;
    };
}

#endif //VD_GAME_ENGINE_MESH_HPP
