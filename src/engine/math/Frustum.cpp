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

    PlaneVec& Frustum::Planes() {
        return m_Planes;
    }

    const PlaneVec& Frustum::Planes() const {
        return m_Planes;
    }

    Frustum::VertexVec& Frustum::Vertices() {
        return m_Vertices;
    }

    const Frustum::VertexVec& Frustum::Vertices() const {
        return m_Vertices;
    }

}