//
// Created by Vali on 9/19/2020.
//

#include "Frustum.hpp"

namespace vd::math {
    Frustum::Frustum()
        : m_Planes(kPlaneCount)
        , m_Vertices(kVertexCount)
    {
    }

    Frustum::~Frustum() = default;

    PlaneVec& Frustum::GetPlanes() {
        return m_Planes;
    }

    const PlaneVec& Frustum::GetPlanes() const {
        return m_Planes;
    }

    void Frustum::SetPlanes(const PlaneVec& planes) {
        m_Planes = planes;
    }

    Frustum::VertexVec& Frustum::GetVertices() {
        return m_Vertices;
    }

    const Frustum::VertexVec& Frustum::GetVertices() const {
        return m_Vertices;
    }

    void Frustum::SetVertices(const Frustum::VertexVec& vertices) {
        m_Vertices = vertices;
    }
}