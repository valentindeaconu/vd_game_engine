//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_FRUSTUM_HPP
#define VD_GAME_ENGINE_FRUSTUM_HPP

#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>

#include <glm/glm.hpp>

#include <memory>

#include "Plane.hpp"
#include "Bounds.hpp"

namespace vd::math {
    class Frustum {
    public:
        enum Plane {
            eNear = 0,
            eFar,
            eBottom,
            eTop,
            eLeft,
            eRight
        };
        static const int kPlaneCount = 6;

        enum Vertex {
            eNearBottomLeft = 0,
            eNearBottomRight,
            eNearTopLeft,
            eNearTopRight,
            eFarBottomLeft,
            eFarBottomRight,
            eFarTopLeft,
            eFarTopRight
        };
        static const int kVertexCount = 8;

        typedef std::vector<glm::vec3>  VertexVec;

        Frustum();
        ~Frustum();

        PlaneVec& Planes();
        [[nodiscard]] const PlaneVec& Planes() const;

        VertexVec& Vertices();
        [[nodiscard]] const VertexVec& Vertices() const;
    private:
        PlaneVec    m_Planes;
        VertexVec   m_Vertices;
    };
    typedef std::shared_ptr<Frustum>    FrustumPtr;
}

#endif //VD_GAME_ENGINE_FRUSTUM_HPP
