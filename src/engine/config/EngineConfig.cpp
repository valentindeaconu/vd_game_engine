#include "EngineConfig.hpp"

namespace vd 
{
	namespace config
	{
		EngineConfig::EngineConfig(const std::string& filePath)
			: ConfigurationFile(filePath)
		{

		}

		EngineConfig::~EngineConfig()
		{

		}

		const glm::vec3& EngineConfig::getFogColor() const
		{
			return fogInfo.color;
		}

		const float& EngineConfig::getFogDensity() const
		{
			return fogInfo.density;
		}
		
		const float& EngineConfig::getFogGradient() const
		{
			return fogInfo.gradient;
		}

		const std::vector<model::LightPtr>& EngineConfig::getLights() const
		{
			return lights;
		}

		void EngineConfig::onTokenReceived(const std::string& command, const std::vector<std::string>& tokens)
		{
			if (command.compare("fogColor") == 0)
			{
				fogInfo.color.r = std::stof(tokens[0]);
				fogInfo.color.g = std::stof(tokens[1]);
				fogInfo.color.b = std::stof(tokens[2]);
			}
			else if (command.compare("fogDensity") == 0)
			{
				fogInfo.density = std::stof(tokens[0]);
			}
			else if (command.compare("fogGradient") == 0)
			{
				fogInfo.gradient = std::stof(tokens[0]);
			}
			else if (command.compare("directional") == 0)
			{
				model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::eDirectional);
				lights.push_back(lightPtr);
			}
			else if (command.compare("point") == 0)
			{
				model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::ePoint);
				lights.push_back(lightPtr);
			}
			else if (command.compare("spot") == 0)
			{
				model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::eSpot);
				lights.push_back(lightPtr);
			}
			else if (command.compare("lightPosition") == 0)
			{
				glm::vec3 position;
				position.x = std::stof(tokens[0]);
				position.y = std::stof(tokens[1]);
				position.z = std::stof(tokens[2]);
				lights.back()->setPosition(position);
			}
			else if (command.compare("lightColor") == 0)
			{
				glm::vec3 color;
				color.r = std::stof(tokens[0]);
				color.g = std::stof(tokens[1]);
				color.b = std::stof(tokens[2]);
				lights.back()->setColor(color);
			}
			else if (command.compare("lightAttenuation") == 0)
			{
				glm::vec3 attenuation;
				attenuation.x = std::stof(tokens[0]);
				attenuation.y = std::stof(tokens[1]);
				attenuation.z = std::stof(tokens[2]);
				lights.back()->setAttenuation(attenuation);
			}
			else if (command.compare("lightAmbientStrength") == 0)
			{
				float ambientStrength = std::stof(tokens[0]);
				lights.back()->setAmbientStrength(ambientStrength);
			}
			else if (command.compare("lightSpecularStrength") == 0)
			{
				float specularStrength = std::stof(tokens[0]);
				lights.back()->setSpecularStrength(specularStrength);
			}
			else if (command.compare("lightShininess") == 0)
			{
				float shininess = std::stof(tokens[0]);
				lights.back()->setShininess(shininess);
			}
		}

		void EngineConfig::onParseFinish()
		{

		}
	}
}