//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERCONFIG_HPP
#define VD_GAME_ENGINE_WATERCONFIG_HPP

#include <engine/config/ConfigurationFile.hpp>
#include <engine/foundation/imgloader/IMGLoader.hpp>
#include <engine/model/Texture.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

namespace mod::water {
    struct WaterPack {
        vd::model::Texture2DPtr dudvMap;
        vd::model::Texture2DPtr normalMap;
    };

    class WaterConfig : public vd::config::ConfigurationFile {
    public:
        explicit WaterConfig(const std::string& filePath);
        ~WaterConfig();

        [[nodiscard]] uint32_t getReflectionWidth() const;
        [[nodiscard]] uint32_t getReflectionHeight() const;

        [[nodiscard]] uint32_t getRefractionWidth() const;
        [[nodiscard]] uint32_t getRefractionHeight() const;

        [[nodiscard]] const vd::model::Texture2DPtr& getDuDvMap(const std::string& name);
        [[nodiscard]] const vd::model::Texture2DPtr& getNormalMap(const std::string& name);

        [[nodiscard]] float getTiling() const;
        [[nodiscard]] float getWaveStrength() const;
        [[nodiscard]] float getWaveSpeed() const;
        [[nodiscard]] float getShineDamper() const;
        [[nodiscard]] float getReflectivity() const;
        [[nodiscard]] const glm::vec4& getBaseColor() const;
    private:
        void onTokenReceived(const std::string& key, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        float tiling;
        float waveStrength;
        float waveSpeed;
        float shineDamper;
        float reflectivity;
        glm::vec4 baseColor;

        uint32_t reflectionWidth;
        uint32_t reflectionHeight;

        uint32_t refractionWidth;
        uint32_t refractionHeight;

        std::unordered_map<std::string, WaterPack>  waterPackMap;

        vd::imgloader::IMGLoaderPtr imgLoaderPtr;
    };
    typedef std::shared_ptr<WaterConfig>    WaterConfigPtr;
}



#endif //VD_GAME_ENGINE_WATERCONFIG_HPP
