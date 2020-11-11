//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_DETECTOR_HPP
#define VD_GAME_ENGINE_DETECTOR_HPP

#include <glm/glm.hpp>

#include <memory>
#include <algorithm>

#include "Bounds.hpp"
#include "Frustum.hpp"
#include "Transform.hpp"

namespace vd::collision {
    enum Relationship {
        eIntersect = 0,
        eInside,
        eOutside
    };

    class Detector {
    public:
        [[nodiscard]] static bool IsPointInsideBounds3(const glm::vec3& point, const vd::math::Bounds3& bounds);
        [[nodiscard]] static Relationship Bounds3AgainstFrustum(const vd::math::Bounds3& bounds,
                                                                const vd::math::Frustum& frustum);
        [[nodiscard]] static bool IsAnyBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                                            const vd::math::Frustum& frustum);
        [[nodiscard]] static bool IsAnyTransformedBounds3InsideFrustum(const std::vector<vd::math::Bounds3>& boundsVec,
                                                                       const vd::math::Transform& transform,
                                                                       const vd::math::Frustum& frustum);
    };
}


#endif //VD_GAME_ENGINE_DETECTOR_HPP
