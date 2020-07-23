#ifndef __LIGHT_HPP_
#define __LIGHT_HPP_

#include <glm/glm.hpp>
#include <memory>

namespace vd::model
{
    class Light
    {
    public:
        enum _type
        {
            eDirectional = 0,
            ePoint,
            eSpot
        };

        Light(const _type& type,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& color = glm::vec3(1.0f),
            const glm::vec3& attenunation = glm::vec3(1.0f),
            float ambientStrength = 0.0f,
            float specularStrength = 0.0f,
            float shininess = 0.0f);
        ~Light();

        const _type& getType() const;

        glm::vec3& getPosition();
        const glm::vec3& getPosition() const;
        void setPosition(const glm::vec3& position);

        glm::vec3& getColor();
        const glm::vec3& getColor() const;
        void setColor(const glm::vec3& color);

        glm::vec3& getAttenuation();
        const glm::vec3& getAttenuation() const;
        void setAttenuation(const glm::vec3& attenuation);

        float& getAmbientStrength();
        const float& getAmbientStrength() const;
        void setAmbientStrength(const float& ambientStrength);

        float& getSpecularStrength();
        const float& getSpecularStrength() const;
        void setSpecularStrength(const float& specularStrength);

        float& getShininess();
        const float& getShininess() const;
        void setShininess(const float& shininess);

    private:
        const _type type;

        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 attenuation;

        float ambientStrength;
        float specularStrength;
        float shininess;
    };
    typedef std::shared_ptr<Light>	LightPtr;
}

#endif // !__LIGHT_HPP_
