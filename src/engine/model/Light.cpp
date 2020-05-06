#include "Light.hpp"

namespace vd
{
	namespace model
	{
		Light::Light(const _type& type, 
			const glm::vec3& position, 
			const glm::vec3& color, 
			const glm::vec3& attenunation,
			float ambientStrength,
			float specularStrength,
			float shininess)
			: type(type)
			, position(position)
			, color(color)
			, attenuation(attenunation)
			, ambientStrength(ambientStrength)
			, specularStrength(specularStrength)
			, shininess(shininess)
		{
		}

		Light::~Light()
		{
		}

		const Light::_type& Light::getType() const
		{
			return type;
		}

		glm::vec3& Light::getPosition()
		{
			return position;
		}

		const glm::vec3& Light::getPosition() const
		{
			return position;
		}

		void Light::setPosition(const glm::vec3& position)
		{
			this->position = position;
		}

		glm::vec3& Light::getColor()
		{
			return color;
		}

		const glm::vec3& Light::getColor() const
		{
			return color;
		}

		void Light::setColor(const glm::vec3& color)
		{
			this->color = color;
		}


		glm::vec3& Light::getAttenuation()
		{
			return attenuation;
		}

		const glm::vec3& Light::getAttenuation() const
		{
			return attenuation;
		}

		void Light::setAttenuation(const glm::vec3& attenuation)
		{
			this->attenuation = attenuation;
		}

		float& Light::getAmbientStrength()
		{
			return ambientStrength;
		}

		const float& Light::getAmbientStrength() const
		{
			return ambientStrength;
		}
		
		void Light::setAmbientStrength(const float& ambientStrength)
		{
			this->ambientStrength = ambientStrength;
		}

		float& Light::getSpecularStrength()
		{
			return specularStrength;
		}

		const float& Light::getSpecularStrength() const
		{
			return specularStrength;
		}
		
		void Light::setSpecularStrength(const float& specularStrength)
		{
			this->specularStrength = specularStrength;
		}

		float& Light::getShininess()
		{
			return shininess;
		}

		const float& Light::getShininess() const
		{
			return shininess;
		}

		void Light::setShininess(const float& shininess)
		{
			this->shininess = shininess;
		}

	}
}