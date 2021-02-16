//
// Created by Vali on 11/11/2020.
//

#include "Detector.hpp"

namespace vd::collision {

    exception::CollisionError::CollisionError(const std::string& message)
        : CustomException("CollisionError", message)
    {
    }

    bool Detector::IsPointInsideBounds2(const glm::vec2& point, const math::Bounds2& bounds) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking against an invalid bound");
        }

        if (bounds.Empty() && point == bounds.Left()) {
            return true;
        }

        return (point.x >= bounds.Left().x && point.x <= bounds.Right().x &&
                point.y >= bounds.Left().y && point.y <= bounds.Right().y);
    }

    Relationship Detector::Bounds2AgainstBounds2(const math::Bounds2& bounds, const math::Bounds2& against) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking an invalid bound");
        }

        if (!against.Valid()) {
            throw exception::CollisionError("Checking against an invalid bound");
        }

        if (bounds.Empty() && against.Empty() && bounds.Left() == against.Right()) {
            return eInside;
        }

        if (bounds.Empty()) {
            return IsPointInsideBounds2(bounds.Left(), against) ? eInside : eOutside;
        }

        if (against.Empty()) {
            return IsPointInsideBounds2(against.Left(), bounds) ? eIntersect : eOutside;
        }

        bool leftInside = IsPointInsideBounds2(bounds.Left(), against);
        bool rightInside = IsPointInsideBounds2(bounds.Right(), against);

        if (leftInside && rightInside) {
            return eInside;
        }

        if (leftInside || rightInside) {
            return eIntersect;
        }

        return eOutside;
    }

    Relationship Detector::Bounds2AgainstFrustum(const math::Bounds2& bounds, const math::Frustum& frustum) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking an invalid bound");
        }

        auto& v0 = frustum.Vertices()[0];

        glm::vec2 mn(v0.x, v0.z);
        glm::vec2 mx(v0.x, v0.z);

        for (int i = 1; i < math::Frustum::kVertexCount; ++i) {
            auto& v = frustum.Vertices()[i];

            mn.x = std::min(mn.x, v.x);
            mn.y = std::min(mn.y, v.z);

            mx.x = std::max(mx.x, v.x);
            mx.y = std::max(mx.y, v.z);
        }

        math::Bounds2 frustum2DProjectionBounds(mn, mx);

        Relationship boundsAndFrustum = Bounds2AgainstBounds2(bounds, frustum2DProjectionBounds);
        if (boundsAndFrustum != eOutside) {
            return boundsAndFrustum;
        }

        // Maybe frustum inside bounds2?
        Relationship frustumAndBounds = Bounds2AgainstBounds2(frustum2DProjectionBounds, bounds);
        if (frustumAndBounds != eOutside) {
            return eIntersect;
        }

        return eOutside;
    }

    /**
     * Based on https://math.stackexchange.com/questions/1472049/check-if-a-point-is-inside-a-rectangular-shaped-area-3d
     *         P6 ------ P7
     *        / |       / |
     *       P5 ----- P8  |
     *       |  |     |	 |
     *       | P2 ----|- P3
     *       | /      |	/
     *       P1 ----- P4
     */
    bool Detector::IsPointInsideBounds3(const glm::vec3& point, const vd::math::Bounds3& bounds) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking against an invalid bound");
        }

        if (bounds.Empty() && bounds.Left() == point) {
            return true;
        }

        glm::vec3 P1(bounds.Left());
        glm::vec3 P2(P1.x, P1.y, bounds.Right().z);
        glm::vec3 P4(bounds.Right().x, P1.y, P1.z);
        glm::vec3 P5(P1.x, bounds.Right().y, P1.z);

        glm::vec3 i = P2 - P1;
        glm::vec3 j = P4 - P1;
        glm::vec3 k = P5 - P1;
        glm::vec3 v = point - P1;

        float dot_v_i = glm::dot(v, i);
        if (dot_v_i > 0 && dot_v_i < glm::dot(i, i)) {
            float dot_v_j = glm::dot(v, j);
            if (dot_v_j > 0 && dot_v_j < glm::dot(j, j)) {
                float dot_v_k = glm::dot(v, k);
                if (dot_v_k > 0 && dot_v_k < glm::dot(k, k)) {
                    return true;
                }
            }
        }

        return false;
    }

    Relationship Detector::Bounds3AgainstBounds3(const math::Bounds3& bounds, const math::Bounds3& against) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking an invalid bound");
        }

        if (!against.Valid()) {
            throw exception::CollisionError("Checking against an invalid bound");
        }

        if (bounds.Empty() && against.Empty() && bounds.Left() == against.Right()) {
            return eInside;
        }

        if (bounds.Empty()) {
            return IsPointInsideBounds3(bounds.Left(), against) ? eInside : eOutside;
        }

        if (against.Empty()) {
            return IsPointInsideBounds3(against.Left(), bounds) ? eIntersect : eOutside;
        }

        bool leftInside = IsPointInsideBounds3(bounds.Left(), against);
        bool rightInside = IsPointInsideBounds3(bounds.Right(), against);

        if (leftInside && rightInside) {
            return eInside;
        }

        if (leftInside || rightInside) {
            return eIntersect;
        }

        return eOutside;
    }

    /**
     * Based on https://stackoverflow.com/questions/52217233/check-if-an-axis-aligned-bounding-box-is-inside-view-frustum
     */
    Relationship Detector::Bounds3AgainstFrustum(const vd::math::Bounds3& bounds, const vd::math::Frustum& frustum) {
        if (!bounds.Valid()) {
            throw exception::CollisionError("Checking an invalid bound");
        }

        if (bounds.Empty()) {
            return eOutside;
        }

        glm::vec3 min, max;

        for (const auto& plane : frustum.Planes()) {
            // X axis
            if(plane.n.x > 0) {
                min.x = bounds.Left().x;
                max.x = bounds.Right().x;
            } else {
                min.x = bounds.Right().x;
                max.x = bounds.Left().x;
            }

            // Y axis
            if(plane.n.y > 0) {
                min.y = bounds.Left().y;
                max.y = bounds.Right().y;
            } else {
                min.y = bounds.Right().y;
                max.y = bounds.Left().y;
            }

            // Z axis
            if(plane.n.z > 0) {
                min.z = bounds.Left().z;
                max.z = bounds.Right().z;
            } else {
                min.z = bounds.Right().z;
                max.z = bounds.Left().z;
            }

            if (glm::dot(plane.n, min) + plane.d > 0)
                return eOutside;

            if (glm::dot(plane.n, max) + plane.d >= 0)
                return eIntersect;
        }

        return eInside;
    }

    bool Detector::IsAnyBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                             const vd::math::Frustum& frustum) {
        for (const auto& bounds : boundsVec) {
            if (Bounds3AgainstFrustum(bounds, frustum) != eOutside) {
                return true;
            }
        }

        return false;
    }

    bool Detector::IsAnyTransformedBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                                        const vd::math::Transform& transform,
                                                        const vd::math::Frustum& frustum) {
        for (const auto& bounds : boundsVec) {
            if (Bounds3AgainstFrustum(bounds.WithTransform(transform), frustum) != eOutside) {
                return true;
            }
        }

        return false;
    }

}