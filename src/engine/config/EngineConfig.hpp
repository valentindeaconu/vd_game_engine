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
        struct FogInfo
        {
            glm::vec3 color;
            float density;
            float gradient;
        };
    }

    class EngineConfig : public ConfigurationFile
    {
    public:
        EngineConfig(const std::string& filePath);
        ~EngineConfig();

        const glm::vec3& getFogColor() const;
        const float& getFogDensity() const;
        const float& getFogGradient() const;

        const std::vector<model::LightPtr>& getLights() const;
    private:
        void onTokenReceived(const std::string& command, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        internal::FogInfo fogInfo;
        std::vector<model::LightPtr> lights;
    };
    typedef std::shared_ptr<EngineConfig>	EngineConfigPtr;
}

#endif // !__ENGINE_CONFIG_HPP_
