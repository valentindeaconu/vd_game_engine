//
// Created by Vali on 11/25/2020.
//

#include "FogManager.hpp"

namespace vd::fog {

    FogManager::FogManager(const std::string& propsFilePath) {
        auto pProperties = vd::loader::PropertiesLoader::Load(propsFilePath);

        m_pFog = std::make_shared<fog::Fog>(
                pProperties->Get<float>("Fog.Density"),
                pProperties->Get<float>("Fog.Gradient"),
                glm::vec3(0.0f)
        );
    }

    void FogManager::Init() {

    }

    void FogManager::Update() {

    }

    void FogManager::CleanUp() {

    }

    void FogManager::AddUniforms(const gl::wrappers::ShaderPtr& pShader) {
        pShader->AddUniform("fog.Density");
        pShader->AddUniform("fog.Gradient");
        pShader->AddUniform("fog.Color");
    }

    void FogManager::SetUniforms(const gl::wrappers::ShaderPtr& pShader) {
        pShader->SetUniform("fog.Density", m_pFog->Density());
        pShader->SetUniform("fog.Gradient", m_pFog->Gradient());
        pShader->SetUniform("fog.Color", m_pFog->Color());
    }

    void FogManager::FogColor(const glm::vec3& color) {
        m_pFog->Color() = color;
    }

    const FogPtr& FogManager::Fog() const {
        return m_pFog;
    }

}