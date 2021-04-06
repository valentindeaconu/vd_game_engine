//
// Created by Vali on 4/3/2021.
//

#include "ParticleFactory.hpp"

namespace mod::particles {

    void ParticleFactory::Create(const vd::EnginePtr& pEngine) {
        ParticleSystemPtr pParticleSystem = std::make_shared<ParticleSystem>(14, 4.2f, 0.02f, 2.45f, "./resources/assets/particles/fire_8x8.png", 8);
        pParticleSystem->RandomizeScale(5.0f, 5.0f);
        pParticleSystem->RandomizeVelocity(-0.17f, 0.17f);
        pParticleSystem->AdditiveBlending() = true;

        ParticleRendererPtr pParticleRenderer = std::make_shared<ParticleRenderer>(pParticleSystem);

        pEngine->Subscribe(pParticleRenderer, ParticleRenderer::kPriority);
    }

}