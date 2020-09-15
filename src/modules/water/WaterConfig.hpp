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

        [[nodiscard]] uint32_t getSize() const;

        [[nodiscard]] uint32_t getReflectionWidth() const;
        [[nodiscard]] uint32_t getReflectionHeight() const;

        [[nodiscard]] uint32_t getRefractionWidth() const;
        [[nodiscard]] uint32_t getRefractionHeight() const;
    private:
        void onTokenReceived(const std::string& key, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        uint32_t size;

        uint32_t reflectionWidth;
        uint32_t reflectionHeight;

        uint32_t refractionWidth;
        uint32_t refractionHeight;
    };
    typedef std::shared_ptr<WaterConfig>    WaterConfigPtr;
}



#endif //VD_GAME_ENGINE_WATERCONFIG_HPP
