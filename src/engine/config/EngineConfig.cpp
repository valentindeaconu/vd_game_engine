#include "EngineConfig.hpp"

namespace vd::config
{
    EngineConfig::EngineConfig(const std::string& filePath)
        : ConfigurationFile(filePath)
    {

    }

    EngineConfig::~EngineConfig() = default;

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
        if (command == "fogColor")
        {
            fogInfo.color.r = std::stof(tokens[0]);
            fogInfo.color.g = std::stof(tokens[1]);
            fogInfo.color.b = std::stof(tokens[2]);
        }
        else if (command == "fogDensity")
        {
            fogInfo.density = std::stof(tokens[0]);
        }
        else if (command == "fogGradient")
        {
            fogInfo.gradient = std::stof(tokens[0]);
        }
        else if (command == "directional")
        {
            model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::eDirectional);
            lights.push_back(lightPtr);
        }
        else if (command == "point")
        {
            model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::ePoint);
            lights.push_back(lightPtr);
        }
        else if (command == "spot")
        {
            model::LightPtr lightPtr = std::make_shared<model::Light>(model::Light::eSpot);
            lights.push_back(lightPtr);
        }
        else if (command == "lightPosition")
        {
            glm::vec3 position;
            position.x = std::stof(tokens[0]);
            position.y = std::stof(tokens[1]);
            position.z = std::stof(tokens[2]);
            lights.back()->setPosition(position);
        }
        else if (command == "lightColor")
        {
            glm::vec3 color;
            color.r = std::stof(tokens[0]);
            color.g = std::stof(tokens[1]);
            color.b = std::stof(tokens[2]);
            lights.back()->setColor(color);
        }
        else if (command == "lightAttenuation")
        {
            glm::vec3 attenuation;
            attenuation.x = std::stof(tokens[0]);
            attenuation.y = std::stof(tokens[1]);
            attenuation.z = std::stof(tokens[2]);
            lights.back()->setAttenuation(attenuation);
        }
        else if (command == "lightAmbientStrength")
        {
            float ambientStrength = std::stof(tokens[0]);
            lights.back()->setAmbientStrength(ambientStrength);
        }
        else if (command == "lightSpecularStrength")
        {
            float specularStrength = std::stof(tokens[0]);
            lights.back()->setSpecularStrength(specularStrength);
        }
        else if (command == "lightShininess")
        {
            float shininess = std::stof(tokens[0]);
            lights.back()->setShininess(shininess);
        }
    }

    void EngineConfig::onParseFinish()
    {
    }
}