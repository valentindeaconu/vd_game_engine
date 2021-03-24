//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_FOG_HPP
#define VD_GAME_ENGINE_FOG_HPP

#include <memory>
#include <glm/glm.hpp>

namespace vd::fog {
    class Fog {
    public:
        explicit Fog(float density, float gradient, const glm::vec3& color);

        float& Density();
        float& Gradient();
        glm::vec3& Color();
    private:
        float m_Density;
        float m_Gradient;

        glm::vec3 m_Color;
    };
    typedef std::shared_ptr<Fog>    FogPtr;
}


#endif //VD_GAME_ENGINE_FOG_HPP
