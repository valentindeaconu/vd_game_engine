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

    int WaterConfig::getSize() const {
        return size;
    }

    void WaterConfig::onTokenReceived(const std::string &key, const std::vector<std::string> &tokens) {
        if (key == "size") {
            size = std::stoi(tokens[0]);
        }
    }

    void WaterConfig::onParseFinish() {

    }


}