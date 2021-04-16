#ifndef __LIGHT_HPP_
#define __LIGHT_HPP_

#include <glm/glm.hpp>
#include <memory>

namespace vd::light {
    enum LightType {
        eDirectional = 0,
        ePoint,
        eSpot
    };

    class Light {
    public:
        explicit Light(const LightType& type,
                       const glm::vec3& position = glm::vec3(0.0f),
                       const glm::vec3& color = glm::vec3(1.0f),
                       const glm::vec3& attenuation = glm::vec3(1.0f),
                       float ambientStrength = 0.0f,
                       float specularStrength = 0.0f,
                       float shininess = 0.0f);
        ~Light();

        [[nodiscard]] const LightType& Type() const;

        glm::vec3& Position();
        glm::vec3& EyeSpacePosition();
        glm::vec3& Color();
        glm::vec3& Attenuation();

        float& AmbientStrength();
        float& SpecularStrength();
        float& Shininess();

    private:
        const LightType m_kType;

        glm::vec3 m_Position;
        glm::vec3 m_EyeSpacePosition;
        glm::vec3 m_Color;
        glm::vec3 m_Attenuation;

        float m_AmbientStrength;
        float m_SpecularStrength;
        float m_Shininess;
    };
    typedef std::shared_ptr<Light>	LightPtr;
}

#endif // !__LIGHT_HPP_
