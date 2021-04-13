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

    /**
     * Returns a value between [toL, toH] which corresponds to a initial value between [fromL, fromH]
     * @param value the initial value
     * @param fromL left margin of the initial interval
     * @param fromH right margin of the initial interval
     * @param toL left margin of the desired interval
     * @param toH right margin of the desired interval
     * @return the corresponding value from the desired interval, mapped linearly
     */
    float Map(float value, float fromL, float fromH, float toL, float toH);

    /**
     * Returns a value between [toL, toH] which corresponds to a initial value between [0, 1]
     * @param value the initial value
     * @param toL left margin of the desired interval
     * @param toH right margin of the desired interval
     * @return the corresponding value from the desired interval, mapped linearly
     */
    float Map01(float value, float toL, float toH);
}


#endif //VDGE_UTILS_HPP
