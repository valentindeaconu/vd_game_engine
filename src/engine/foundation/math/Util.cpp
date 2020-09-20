//
// Created by Vali on 9/19/2020.
//

#include "Util.hpp"

namespace vd::math {
    bool isPointInsideBoundingBox(const glm::vec3& point, const BoundingBox& boundingBox) {
        // bibliography: https://math.stackexchange.com/questions/1472049/check-if-a-point-is-inside-a-rectangular-shaped-area-3d
        /*
              P6 ------ P7
             / |       / |
            P5 ----- P8  |
            |  |     |	 |
            | P2 ----|- P3
            | /      |	/
            P1 ----- P4
        */
        glm::vec3 P1(boundingBox.getBottomLeft());
        glm::vec3 P2(P1.x, P1.y, boundingBox.getTopRight().z);
        glm::vec3 P4(boundingBox.getTopRight().x, P1.y, P1.z);
        glm::vec3 P5(P1.x, boundingBox.getTopRight().y, P1.z);

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
}