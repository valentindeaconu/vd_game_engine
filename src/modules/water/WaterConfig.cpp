//
// Created by Vali on 9/11/2020.
//

#include "WaterConfig.hpp"

namespace mod::water {

    WaterConfig::WaterConfig(const std::string &filePath)
        : ConfigurationFile(filePath)
        , size(0)
    {
    }

    WaterConfig::~WaterConfig() = default;

    uint32_t WaterConfig::getSize() const {
        return size;
    }

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

    void WaterConfig::onTokenReceived(const std::string &key, const std::vector<std::string> &tokens) {
        if (key == "size") {
            size = std::stoi(tokens[0]);
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
    }

    void WaterConfig::onParseFinish() {

    }


}