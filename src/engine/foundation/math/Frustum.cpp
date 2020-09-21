//
// Created by Vali on 9/19/2020.
//

#include "Frustum.hpp"

namespace vd::math {

    Frustum::Frustum(const core::WindowPtr& windowPtr, const core::CameraPtr& cameraPtr)
        : windowPtr(windowPtr)
        , cameraPtr(cameraPtr)
        , planes(kPlaneCount)
        , vertices(kVertexCount)
    {
        updateWidthsAndHeights();
    }

    Frustum::~Frustum() = default;

    void Frustum::init() {
        updateFrustumVertices();
        updatePlanes();
    }

    void Frustum::update() {
        // TODO: As optimization, this function should be called only if camera perspective is changed or window is resized

        updateFrustumVertices();
        updatePlanes();
    }

    /**
     * Based on https://stackoverflow.com/questions/52217233/check-if-an-axis-aligned-bounding-box-is-inside-view-frustum
     */
    Frustum::CollisionRelationship Frustum::checkAgainst(const BoundingBox& boundingBox) const {
        if (boundingBox.isEmpty())
            return eOutside;

        glm::vec3 min, max;

        for (const auto& plane : planes) {
            // X axis
            if(plane.n.x > 0) {
                min.x = boundingBox.getBottomLeft().x;
                max.x = boundingBox.getTopRight().x;
            } else {
                min.x = boundingBox.getTopRight().x;
                max.x = boundingBox.getBottomLeft().x;
            }

            // Y axis
            if(plane.n.y > 0) {
                min.y = boundingBox.getBottomLeft().y;
                max.y = boundingBox.getTopRight().y;
            } else {
                min.y = boundingBox.getTopRight().y;
                max.y = boundingBox.getBottomLeft().y;
            }

            // Z axis
            if(plane.n.z > 0) {
                min.z = boundingBox.getBottomLeft().z;
                max.z = boundingBox.getTopRight().z;
            } else {
                min.z = boundingBox.getTopRight().z;
                max.z = boundingBox.getBottomLeft().z;
            }

            if (glm::dot(plane.n, min) + plane.d > 0)
                return eOutside;

            //if (glm::dot(plane.n, max) + plane.d >= 0)
            //    return eIntersect;
        }

        return eInside;
    }

    void Frustum::updatePlanes() {
        planes[eLeft] = computePlane(vertices[eFarBottomLeft],
                                     vertices[eNearBottomLeft],
                                     vertices[eFarTopLeft],
                                     vertices[eNearTopLeft]);

        planes[eRight] = computePlane(vertices[eNearBottomRight],
                                      vertices[eFarBottomRight],
                                      vertices[eNearTopRight],
                                      vertices[eFarTopRight]);

        planes[eNear] = computePlane(vertices[eNearBottomLeft],
                                     vertices[eNearBottomRight],
                                     vertices[eNearTopLeft],
                                     vertices[eNearTopRight]);

        planes[eFar] = computePlane(vertices[eFarBottomRight],
                                     vertices[eFarBottomLeft],
                                     vertices[eFarTopRight],
                                     vertices[eFarTopLeft]);

        planes[eTop] = computePlane(vertices[eNearTopLeft],
                                    vertices[eNearTopRight],
                                    vertices[eFarTopLeft],
                                    vertices[eFarTopRight]);

        planes[eBottom] = computePlane(vertices[eFarBottomLeft],
                                    vertices[eFarBottomRight],
                                    vertices[eNearBottomLeft],
                                    vertices[eNearBottomRight]);
    }

    Frustum::Plane Frustum::computePlane(const glm::vec3& bl,
                                         const glm::vec3& br,
                                         const glm::vec3& tl,
                                         const glm::vec3& tr) const {
        glm::vec3 bl_to_br = br - bl;
        glm::vec3 bl_to_tl = tl - bl;

        Frustum::Plane plane;

        plane.n = glm::normalize(glm::cross(bl_to_br, bl_to_tl));
        plane.d = -(plane.n.x * tr.x + plane.n.y * tr.y + plane.n.z * tr.z);

        return plane;
    }

    void Frustum::updateFrustumVertices() {
        glm::vec3 cameraPosition = cameraPtr->getPosition();
        glm::vec3 forward = cameraPtr->getDirection();
        glm::vec3 right = cameraPtr->getRightDirection();
        glm::vec3 up = cameraPtr->getUpDirection();

        float zNear = windowPtr->getNearPlane();
        float zFar = windowPtr->getFarPlane();

        // Compute center points
        glm::vec3 fc = cameraPosition + (forward * zFar); // far plane center point
        glm::vec3 nc = cameraPosition + (forward * zNear); // near plane center point

        if (windowPtr->isPerspectiveChanged()) {
            updateWidthsAndHeights();
        }

        vertices[eNearBottomLeft] = nc - up*near.height - right*near.width;
        vertices[eNearBottomRight] = nc - up*near.height + right*near.width;
        vertices[eNearTopLeft] = nc + up*near.height - right*near.width;
        vertices[eNearTopRight] = nc + up*near.height + right*near.width;
        vertices[eFarBottomLeft] = fc - up*far.height - right*far.width;
        vertices[eFarBottomRight] = fc - up*far.height + right*far.width;
        vertices[eFarTopLeft] = fc + up*far.height - right*far.width;
        vertices[eFarTopRight] = fc + up*far.height + right*far.width;
    }

    void Frustum::updateWidthsAndHeights() {
        float zNear = windowPtr->getNearPlane();
        float zFar = windowPtr->getFarPlane();
        float fovY = windowPtr->getFieldOfView();
        float aspect = windowPtr->getAspectRatio();

        far.width = zFar * std::tan(glm::radians(fovY));
        near.width = zNear * std::tan(glm::radians(fovY));
        far.height = far.width / aspect;
        near.height = near.width / aspect;
    }
}