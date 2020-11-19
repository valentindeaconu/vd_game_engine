//
// Created by Vali on 11/10/2020.
//

#include "LightManager.hpp"

namespace vd::light {
    LightManager::LightManager()
        : m_pSun(nullptr)
        , m_Lights()
    {
    }

    LightManager::~LightManager() = default;

    void LightManager::Link() {
        auto& pProps = ObjectOfType<property::GlobalProperties>::Find();

        m_pSun = std::make_shared<Light>(
                LightType::eDirectional,
                pProps->Get<glm::vec3>("Sun.Position"),
                pProps->Get<glm::vec3>("Sun.Color"),
                glm::vec3(0.0f),
                pProps->Get<float>("Sun.AmbientStrength"),
                pProps->Get<float>("Sun.SpecularStrength"),
                pProps->Get<float>("Sun.Shininess")
        );

        for (int i = 0; ; ++i) {
            const std::string prefix = "Light." + std::to_string(i);
            try {
                LightType type = eDirectional;
                auto type_str = pProps->Get<std::string>(prefix + ".Type");
                if (type_str == "point") {
                    type = ePoint;
                } else if (type_str == "spot") {
                    type = eSpot;
                }

                m_Lights.emplace_back(std::make_shared<Light>(type,
                                                              pProps->Get<glm::vec3>(prefix + ".Position"),
                                                              pProps->Get<glm::vec3>(prefix + ".Color"),
                                                              pProps->Get<glm::vec3>(prefix + ".Attenuation"),
                                                              pProps->Get<float>(prefix + ".AmbientStrength"),
                                                              pProps->Get<float>(prefix + ".SpecularStrength"),
                                                              pProps->Get<float>(prefix + ".Shininess")
                ));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void LightManager::Init() {

    }

    void LightManager::Update() {

    }

    void LightManager::CleanUp() {

    }

    const LightPtr &LightManager::Sun() const {
        return m_pSun;
    }

    const std::vector<LightPtr>& LightManager::Lights() const {
        return m_Lights;
    }
}