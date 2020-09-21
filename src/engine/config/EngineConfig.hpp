#ifndef __ENGINE_CONFIG_HPP_
#define __ENGINE_CONFIG_HPP_

#include <glm/glm.hpp>
#include <vector>

#include "ConfigurationFile.hpp"

#include <engine/model/Light.hpp>

namespace vd::config
{
    namespace internal
    {
        struct FogInfo {
            glm::vec3 color;
            float density;
            float skyDensity;
            float gradient;
        };

        struct ShadowInfo {
            int mapSize;
            float distance;
            float transitionDistance;
            float offset;
        };

        struct CameraInfo {
            glm::vec3 position;
            glm::vec3 target;
            float speed;
        };
    }

    class EngineConfig : public ConfigurationFile
    {
    public:
        explicit EngineConfig(const std::string& filePath);
        ~EngineConfig();

        [[nodiscard]] int getShadowMapSize() const;
        [[nodiscard]] float getShadowDistance() const;
        [[nodiscard]] float getShadowTransitionDistance() const;
        [[nodiscard]] float getShadowOffset() const;

        [[nodiscard]] const glm::vec3& getCameraPosition() const;
        [[nodiscard]] const glm::vec3& getCameraTarget() const;
        [[nodiscard]] float getCameraSpeed() const;

        [[nodiscard]] const glm::vec3& getFogColor() const;
        [[nodiscard]] const float& getFogDensity() const;
        [[nodiscard]] const float& getFogSkyDensity() const;
        [[nodiscard]] const float& getFogGradient() const;

        [[nodiscard]] const std::vector<model::LightPtr>& getLights() const;
    private:
        void onTokenReceived(const std::string& command, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        internal::FogInfo fogInfo;
        internal::ShadowInfo shadowInfo;
        internal::CameraInfo cameraInfo;
        std::vector<model::LightPtr> lights;
    };
    typedef std::shared_ptr<EngineConfig>	EngineConfigPtr;
}

#endif // !__ENGINE_CONFIG_HPP_
