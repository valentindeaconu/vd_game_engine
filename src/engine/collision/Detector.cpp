//
// Created by Vali on 11/11/2020.
//

#include "Detector.hpp"

namespace vd::collision {
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
        glm::vec3 P1(bounds.GetLeft());
        glm::vec3 P2(P1.x, P1.y, bounds.GetRight().z);
        glm::vec3 P4(bounds.GetRight().x, P1.y, P1.z);
        glm::vec3 P5(P1.x, bounds.GetRight().y, P1.z);

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

    /**
     * Based on https://stackoverflow.com/questions/52217233/check-if-an-axis-aligned-bounding-box-is-inside-view-frustum
     */
    Relationship Detector::Bounds3AgainstFrustum(const vd::math::Bounds3& bounds, const vd::math::Frustum& frustum) {
        if (bounds.IsEmpty())
            return eOutside;

        glm::vec3 min, max;

        for (const auto& plane : frustum.GetPlanes()) {
            // X axis
            if(plane.n.x > 0) {
                min.x = bounds.GetLeft().x;
                max.x = bounds.GetRight().x;
            } else {
                min.x = bounds.GetRight().x;
                max.x = bounds.GetLeft().x;
            }

            // Y axis
            if(plane.n.y > 0) {
                min.y = bounds.GetLeft().y;
                max.y = bounds.GetRight().y;
            } else {
                min.y = bounds.GetRight().y;
                max.y = bounds.GetLeft().y;
            }

            // Z axis
            if(plane.n.z > 0) {
                min.z = bounds.GetLeft().z;
                max.z = bounds.GetRight().z;
            } else {
                min.z = bounds.GetRight().z;
                max.z = bounds.GetLeft().z;
            }

            if (glm::dot(plane.n, min) + plane.d > 0)
                return eOutside;
        }

        return eInside;
    }

    bool Detector::IsAnyBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                             const vd::math::Frustum& frustum) {
        return std::ranges::any_of(boundsVec.cbegin(), boundsVec.cend(), [&](const math::Bounds3& bounds) {
            return Bounds3AgainstFrustum(bounds, frustum) != eOutside;
        });
    }

    bool Detector::IsAnyTransformedBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                                        const vd::math::Transform& transform,
                                                        const vd::math::Frustum& frustum) {
        return std::ranges::any_of(boundsVec.cbegin(), boundsVec.cend(), [&](const math::Bounds3& bounds) {
            return Bounds3AgainstFrustum(bounds.withTransform(transform), frustum) != eOutside;
        });
    }

}