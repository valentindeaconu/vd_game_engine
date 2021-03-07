//
// Created by Vali on 3/7/2021.
//

#ifndef VDGE_UTILS_HPP
#define VDGE_UTILS_HPP

#include <glm/glm.hpp>

#include <limits>

namespace vd::math {
    const float g_kEpsilon = std::numeric_limits<float>::epsilon();

    /**
     * Returns a value between [0, 360) which corresponds to a given angle
     * @param angle the angle
     * @return the reduced angle
     */
    float ReduceAngle(float angle);
}


#endif //VDGE_UTILS_HPP
