//
// Created by Vali on 9/11/2020.
//

#include "WaterConfig.hpp"

namespace mod::water {

    WaterConfig::WaterConfig(const std::string &filePath)
        : ConfigurationFile(filePath)
    {
    }

    WaterConfig::~WaterConfig() = default;

    uint32_t WaterConfig::getReflectionWidth() const {
        return reflectionWidth;
    }

    uint32_t WaterConfig::getReflectionHeight() const {
        return reflectionHeight;
    }

    uint32_t WaterConfig::getRefractionWidth() const {
        return refractionWidth;
    }

    uint32_t WaterConfig::getRefractionHeight() const {
        return refractionHeight;
    }

    const vd::model::Texture2DPtr& WaterConfig::getDuDvMap(const std::string& name) {
        return waterPackMap[name].dudvMap;
    }

    const vd::model::Texture2DPtr& WaterConfig::getNormalMap(const std::string& name) {
        return waterPackMap[name].normalMap;
    }

    float WaterConfig::getTiling() const {
        return tiling;
    }

    float WaterConfig::getWaveStrength() const {
        return waveStrength;
    }

    float WaterConfig::getWaveSpeed() const {
        return waveSpeed;
    }

    float WaterConfig::getShineDamper() const {
        return shineDamper;
    }

    float WaterConfig::getReflectivity() const {
        return reflectivity;
    }

    const glm::vec4& WaterConfig::getBaseColor() const {
        return baseColor;
    }

    void WaterConfig::onTokenReceived(const std::string &key, const std::vector<std::string> &tokens) {
        if (key == "tiling") {
            tiling = std::stof(tokens[0]);
        }
        else if (key == "waveStrength") {
            waveStrength = std::stof(tokens[0]);
        }
        else if (key == "waveSpeed") {
            waveSpeed = std::stof(tokens[0]);
        }
        else if (key == "shineDamper") {
            shineDamper = std::stof(tokens[0]);
        }
        else if (key == "reflectivity") {
            reflectivity = std::stof(tokens[0]);
        }
        else if (key == "baseColor") {
            baseColor = glm::vec4(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]), 1.0f);
        }
        else if (key == "reflectionWidth") {
            reflectionWidth = std::stoi(tokens[0]);
        }
        else if (key == "reflectionHeight") {
            reflectionHeight = std::stoi(tokens[0]);
        }
        else if (key == "refractionWidth") {
            refractionWidth = std::stoi(tokens[0]);
        }
        else if (key == "refractionHeight") {
            refractionHeight = std::stoi(tokens[0]);
        }
        else if (key == "pack") {
            waterPackMap[tokens[0]] = {
                .dudvMap = vd::model::TextureService::get(tokens[1]),
                .normalMap = vd::model::TextureService::get(tokens[2])
            };


        }
    }

    void WaterConfig::onParseFinish() {

    }


}