//
// Created by Vali on 11/10/2020.
//

#include "LightManager.hpp"

namespace vd::light {
    LightManager::LightManager()
        : m_Sun(nullptr)
        , m_Lights()
    {
    }

    LightManager::~LightManager() = default;

    void LightManager::Init() {
        auto& propertiesPtr = ObjectOfType<misc::Properties>::Find();

        m_Sun = std::make_shared<Light>(
                LightType::eDirectional,
                propertiesPtr->Get<glm::vec3>("Sun.Position"),
                propertiesPtr->Get<glm::vec3>("Sun.Color"),
                glm::vec3(0.0f),
                propertiesPtr->Get<float>("Sun.AmbientStrength"),
                propertiesPtr->Get<float>("Sun.SpecularStrength"),
                propertiesPtr->Get<float>("Sun.Shininess")
        );

        for (int i = 0; ; ++i) {
            const std::string prefix = "Light." + std::to_string(i);
            try {
                LightType type = eDirectional;
                auto type_str = propertiesPtr->Get<std::string>(prefix + ".Type");
                if (type_str == "point") {
                    type = ePoint;
                } else if (type_str == "spot") {
                    type = eSpot;
                }

                m_Lights.emplace_back(std::make_shared<Light>(type,
                        propertiesPtr->Get<glm::vec3>(prefix + ".Position"),
                        propertiesPtr->Get<glm::vec3>(prefix + ".Color"),
                        propertiesPtr->Get<glm::vec3>(prefix + ".Attenuation"),
                        propertiesPtr->Get<float>(prefix + ".AmbientStrength"),
                        propertiesPtr->Get<float>(prefix + ".SpecularStrength"),
                        propertiesPtr->Get<float>(prefix + ".Shininess")
                ));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void LightManager::Update() {

    }

    void LightManager::CleanUp() {

    }

    const LightPtr &LightManager::GetSun() const {
        return m_Sun;
    }

    const std::vector<LightPtr>& LightManager::GetLights() const {
        return m_Lights;
    }
}