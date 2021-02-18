//
// Created by Vali on 12/1/2020.
//

#include "Mesh.hpp"

namespace vd::model {

    Mesh2D::Mesh2D()
        : m_Vertices()
        , m_Indices()
        , m_Materials()
    {
    }

    Mesh2D::~Mesh2D() = default;

    Mesh2D::VertexVec& Mesh2D::Vertices() {
        return m_Vertices;
    }

    Mesh2D::IndexVec& Mesh2D::Indices() {
        return m_Indices;
    }

    Mesh2D::MaterialVec& Mesh2D::Materials() {
        return m_Materials;
    }

    Mesh3D::Mesh3D()
        : m_Vertices()
        , m_Indices()
        , m_Materials()
    {
    }

    Mesh3D::~Mesh3D() = default;

    Mesh3D::VertexVec& Mesh3D::Vertices() {
        return m_Vertices;
    }

    Mesh3D::IndexVec& Mesh3D::Indices() {
        return m_Indices;
    }

    Mesh3D::MaterialVec& Mesh3D::Materials() {
        return m_Materials;
    }
}
