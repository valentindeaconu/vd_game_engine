//
// Created by Vali on 12/1/2020.
//

#include "Mesh.hpp"

namespace vd::model {

    model::Mesh::Mesh()
        : m_Vertices()
        , m_Indices()
        , m_Materials()
    {
    }

    Mesh::~Mesh() = default;

    Mesh::VertexVec& Mesh::Vertices() {
        return m_Vertices;
    }

    Mesh::IndexVec& Mesh::Indices() {
        return m_Indices;
    }

    Mesh::MaterialVec& Mesh::Materials() {
        return m_Materials;
    }
}
