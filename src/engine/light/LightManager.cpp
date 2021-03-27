//
// Created by Vali on 11/10/2020.
//

#include "LightManager.hpp"

namespace vd::light {
    LightManager::LightManager(const std::string& propsFilePath) {
        auto pProperties = vd::loader::PropertiesLoader::Load(propsFilePath);

        m_pSun = std::make_shared<Light>(
                LightType::eDirectional,
                pProperties->Get<glm::vec3>("Sun.Position"),
                pProperties->Get<glm::vec3>("Sun.Color"),
                glm::vec3(0.0f),
                pProperties->Get<float>("Sun.AmbientStrength"),
                pProperties->Get<float>("Sun.SpecularStrength"),
                pProperties->Get<float>("Sun.Shininess")
        );

        for (int i = 0; ; ++i) {
            const std::string prefix = "Light." + std::to_string(i);
            try {
                LightType type = eDirectional;
                auto type_str = pProperties->Get<std::string>(prefix + ".Type");
                if (type_str == "point") {
                    type = ePoint;
                } else if (type_str == "spot") {
                    type = eSpot;
                }

                m_Lights.emplace_back(std::make_shared<Light>(type,
                                                              pProperties->Get<glm::vec3>(prefix + ".Position"),
                                                              pProperties->Get<glm::vec3>(prefix + ".Color"),
                                                              pProperties->Get<glm::vec3>(prefix + ".Attenuation"),
                                                              pProperties->Get<float>(prefix + ".AmbientStrength"),
                                                              pProperties->Get<float>(prefix + ".SpecularStrength"),
                                                              pProperties->Get<float>(prefix + ".Shininess")
                ));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void LightManager::Link() {
        m_pCamera = vd::ObjectOfType<camera::Camera>::Find();
    }

    void LightManager::Init() {
        Update();
    }

    void LightManager::Update() {
        if (m_pCamera->CameraRotated() || m_pCamera->CameraMoved()) {
            // TODO: Dispatch this computation on a job thread
            // m_pThreadPool->CreateJob([&, view = m_pCamera->ViewMatrix()]() {
                m_LightDirectionMatrix = glm::transpose(glm::inverse(glm::mat3(m_pCamera->ViewMatrix())));
            // }, true);
        }
    }

    void LightManager::CleanUp() {

    }

    void LightManager::AddUniforms(const gl::ShaderPtr& pShader) {
        pShader->AddUniform("sun.Direction");
        pShader->AddUniform("sun.Color");
        pShader->AddUniform("sun.AmbientStrength");
        pShader->AddUniform("sun.SpecularStrength");
        pShader->AddUniform("sun.Shininess");

        for (size_t i = 0; i < m_Lights.size(); ++i) {
            std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

            pShader->AddUniform(currentLightUniformNameBase + ".Type");
            pShader->AddUniform(currentLightUniformNameBase + ".Position");
            pShader->AddUniform(currentLightUniformNameBase + ".Direction");
            pShader->AddUniform(currentLightUniformNameBase + ".Color");
            pShader->AddUniform(currentLightUniformNameBase + ".Attenuation");
            pShader->AddUniform(currentLightUniformNameBase + ".AmbientStrength");
            pShader->AddUniform(currentLightUniformNameBase + ".SpecularStrength");
            pShader->AddUniform(currentLightUniformNameBase + ".Shininess");
        }

        pShader->AddUniform("lightDirectionMatrix");
    }

    void LightManager::SetUniforms(const gl::ShaderPtr& pShader) {
        pShader->SetUniform("sun.Direction", m_pSun->Direction());
        pShader->SetUniform("sun.Color", m_pSun->Color());
        pShader->SetUniform("sun.AmbientStrength", m_pSun->AmbientStrength());
        pShader->SetUniform("sun.SpecularStrength", m_pSun->SpecularStrength());
        pShader->SetUniform("sun.Shininess", m_pSun->Shininess());

        for (size_t i = 0; i < m_Lights.size(); ++i) {
            auto& pLight = m_Lights[i];

            std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

            switch (pLight->Type()) {
                case vd::light::eDirectional: {
                    pShader->SetUniform(currentLightUniformNameBase + ".Type", 0);
                    pShader->SetUniform(currentLightUniformNameBase + ".Direction", pLight->Direction());
                    break;
                }
                case vd::light::ePoint: {
                    pShader->SetUniform(currentLightUniformNameBase + ".Type", 1);
                    pShader->SetUniform(currentLightUniformNameBase + ".Position", pLight->Position());
                    break;
                }
                case vd::light::eSpot:
                default: {
                    pShader->SetUniform(currentLightUniformNameBase + ".Type", 2);
                    pShader->SetUniform(currentLightUniformNameBase + ".Position", pLight->Position());
                    break;
                }
            }
            pShader->SetUniform(currentLightUniformNameBase + ".Color", pLight->Color());
            pShader->SetUniform(currentLightUniformNameBase + ".Attenuation", pLight->Attenuation());
            pShader->SetUniform(currentLightUniformNameBase + ".AmbientStrength", pLight->AmbientStrength());
            pShader->SetUniform(currentLightUniformNameBase + ".SpecularStrength", pLight->SpecularStrength());
            pShader->SetUniform(currentLightUniformNameBase + ".Shininess", pLight->Shininess());
        }

        pShader->SetUniform("lightDirectionMatrix", m_LightDirectionMatrix);
    }

    const LightPtr& LightManager::Sun() const {
        return m_pSun;
    }

    const std::vector<LightPtr>& LightManager::Lights() const {
        return m_Lights;
    }

}