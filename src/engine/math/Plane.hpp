//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_PLANE_HPP
#define VD_GAME_ENGINE_PLANE_HPP

#include <glm/glm.hpp>

#include <vector>

namespace vd::math {
    struct Plane {
        Plane();
        Plane(const glm::vec3& n, float d);
        Plane(const glm::vec3& bl, const glm::vec3& br, const glm::vec3& tl, const glm::vec3& tr);

        glm::vec3 n;
        float d;
    };
    typedef std::vector<Plane> PlaneVec;
}


#endif //VD_GAME_ENGINE_PLANE_HPP
