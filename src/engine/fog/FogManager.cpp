//
// Created by Vali on 11/25/2020.
//

#include "FogManager.hpp"

namespace vd::fog {

    FogManager::FogManager(const std::string& propsFilePath) {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        m_pFog = std::make_shared<fog::Fog>(
                pProps->Get<float>("Fog.Density"),
                pProps->Get<float>("Fog.SkyDensity"),
                pProps->Get<float>("Fog.Gradient"),
                pProps->Get<glm::vec3>("Fog.Color")
        );
    }

    FogManager::~FogManager() = default;

    void FogManager::Init() {

    }

    void FogManager::Update() {

    }

    void FogManager::CleanUp() {

    }

    void FogManager::AddUniforms(const gl::ShaderPtr& pShader) {
        SKIP_IF_EXCEPTION(pShader->AddUniform("fog.Density"));
        SKIP_IF_EXCEPTION(pShader->AddUniform("fog.SkyDensity"));
        SKIP_IF_EXCEPTION(pShader->AddUniform("fog.Gradient"));
        SKIP_IF_EXCEPTION(pShader->AddUniform("fog.Color"));
    }

    void FogManager::SetUniforms(const gl::ShaderPtr& pShader) {
        SKIP_IF_EXCEPTION(pShader->SetUniform("fog.Density", m_pFog->Density()));
        SKIP_IF_EXCEPTION(pShader->SetUniform("fog.SkyDensity", m_pFog->SkyDensity()));
        SKIP_IF_EXCEPTION(pShader->SetUniform("fog.Gradient", m_pFog->Gradient()));
        SKIP_IF_EXCEPTION(pShader->SetUniform("fog.Color", m_pFog->Color()));
    }

    const FogPtr& FogManager::Fog() const {
        return m_pFog;
    }

}