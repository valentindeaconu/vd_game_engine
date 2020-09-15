//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERCONFIG_HPP
#define VD_GAME_ENGINE_WATERCONFIG_HPP

#include <engine/config/ConfigurationFile.hpp>

#include <memory>

namespace mod::water {
    class WaterConfig : public vd::config::ConfigurationFile {
    public:
        explicit WaterConfig(const std::string& filePath);
        ~WaterConfig();

        [[nodiscard]] int getSize() const;
    private:
        void onTokenReceived(const std::string& key, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        int size;
    };
    typedef std::shared_ptr<WaterConfig>    WaterConfigPtr;
}



#endif //VD_GAME_ENGINE_WATERCONFIG_HPP
