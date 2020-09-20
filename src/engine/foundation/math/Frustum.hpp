//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_FRUSTUM_HPP
#define VD_GAME_ENGINE_FRUSTUM_HPP

#include <engine/core/Window.hpp>
#include <engine/core/Camera.hpp>

#include <glm/glm.hpp>

#include <memory>

#include "BoundingBox.hpp"

namespace vd::math {
    class Frustum {
    public:
        enum CollisionRelationship {
            eIntersect = 0,
            eInside,
            eOutside
        };

        Frustum(const core::WindowPtr& windowPtr, const core::CameraPtr& cameraPtr);
        ~Frustum();

        void init();
        void update();

        [[nodiscard]] CollisionRelationship checkAgainst(const BoundingBox& boundingBox) const;
    private:
        const int kPlaneCount = 6;
        const int kVertexCount = 8;

        enum FrustumPlane {
            eNear = 0,
            eFar,
            eBottom,
            eTop,
            eLeft,
            eRight
        };

        enum FrustumVertex {
            eNearBottomLeft = 0,
            eNearBottomRight,
            eNearTopLeft,
            eNearTopRight,
            eFarBottomLeft,
            eFarBottomRight,
            eFarTopLeft,
            eFarTopRight
        };

        struct Plane {
            glm::vec3 n;
            float d;
        };
        typedef std::vector<Plane>      PlaneVec;
        typedef std::vector<glm::vec3>  VertexVec;

        void updatePlanes();
        [[nodiscard]] Plane computePlane(const glm::vec3& bl,
                                         const glm::vec3& br,
                                         const glm::vec3& tl,
                                         const glm::vec3& tr) const;

        void updateFrustumVertices();
        void updateWidthsAndHeights();

        struct {
            float width;
            float height;
        } near, far;

        PlaneVec planes;
        VertexVec vertices;

        core::WindowPtr windowPtr;
        core::CameraPtr cameraPtr;
    };
    typedef std::shared_ptr<Frustum>    FrustumPtr;
}

#endif //VD_GAME_ENGINE_FRUSTUM_HPP
