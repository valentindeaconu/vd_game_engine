//
// Created by Vali on 3/7/2021.
//

#include "Utils.hpp"

namespace vd::math {

    float ReduceAngle(float angle) {
        const float inv360 = 0.0027777777777778f; // 1 / 360.0f

        float scale = glm::abs(angle * inv360);

        if (scale < 1.0f) {
            return (angle < 0.0f) ? (360.0f - angle) : angle;
        } else {
            float factor = glm::floor(scale) * 360.0f;
            if (angle < 0.0f) {
                angle += factor;
                return 360.0f + angle;
            } else {
                angle -= factor;
                return angle;
            }
        }
    }

    float Map(float value, float fromL, float fromH, float toL, float toH) {
        return (value - fromL) * (toH - toL) / (fromH - fromL) + toL;
    }

    float Map01(float value, float toL, float toH) {
        return value * (toH - toL) + toL;
    }
}