//
// Created by Vali on 4/3/2021.
//

#include "ParticleFactory.hpp"

namespace mod::particles {

    void ParticleFactory::Create(const vd::EnginePtr& pEngine) {

        // User Smoke 
        {
            ParticleSystemPtr pParticleSystem = std::make_shared<ParticleSystem>(7, 1.25f, -0.01f, 1.65f, "./resources/assets/particles/particleAtlas_4x4.png", 4);
            pParticleSystem->RandomizeScale(0.15f, 0.23f);
            pParticleSystem->RandomizeVelocity(-0.35f, 0.35f);
            pParticleSystem->AdditiveBlending() = true;

            ParticleRendererPtr pParticleRenderer = std::make_shared<ParticleRenderer>(
                pParticleSystem, 
                [p = vd::ObjectOfType<mod::player::Player>::Find()](ParticleSystemPtr sys, float deltaTime, ParticlePtrVec& particles) {
                    glm::vec3 systemCenter = p->WorldTransform().Translation();
                    systemCenter.y -= p->ModelYOffset();
                    sys->GenerateParticles(systemCenter, 0.65f, deltaTime, particles);
                }
            );

            pEngine->Subscribe(pParticleRenderer, ParticleRenderer::kPriority);
        }

        // Rain
        {
            ParticleSystemPtr pParticleSystem = std::make_shared<ParticleSystem>(300, 4.2f, 0.85f, 2.50f, "./resources/assets/particles/rain_5x5.png", 5);
            pParticleSystem->RandomizeScale(0.65f, 0.75f);
            pParticleSystem->RandomizeVelocity(-0.70f, -0.35f);
            pParticleSystem->RandomizeRotation(90.0f, 90.0f);
            pParticleSystem->AdditiveBlending() = true;

            ParticleRendererPtr pParticleRenderer = std::make_shared<ParticleRenderer>(
                pParticleSystem, 
                [c = vd::ObjectOfType<vd::camera::Camera>::Find()](ParticleSystemPtr sys, float deltaTime, ParticlePtrVec& particles) {
                    static const float kHeightOffset = 5.0f;
                    static const float kDistanceFromCamera = 35.0f; 
                    static const float kSystemRadius = 50.0f;

                    const auto& eyePosition = c->Position();
                    const auto& eyeForward = c->Forward();

                    glm::vec3 systemCenter = eyePosition + (eyeForward * (kDistanceFromCamera + kSystemRadius));

                    systemCenter.y = eyePosition.y + kHeightOffset;

                    sys->GenerateParticles(systemCenter, kSystemRadius, deltaTime, particles);
                }
            );

            pEngine->Subscribe(pParticleRenderer, ParticleRenderer::kPriority);
        }
    }

}