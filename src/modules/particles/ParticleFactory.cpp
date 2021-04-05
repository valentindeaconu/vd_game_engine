//
// Created by Vali on 4/3/2021.
//

#include "ParticleFactory.hpp"

namespace mod::particles {

    void ParticleFactory::Create(const vd::EnginePtr& pEngine) {
        ParticleShaderPtr pParticleShader = std::make_shared<ParticleShader>();

        ParticleRendererPtr pParticleRenderer = std::make_shared<ParticleRenderer>(pParticleShader,
                                                                                   []() {
                                                                                       glEnable(GL_BLEND);
                                                                                       glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                                                                                       glDepthMask(GL_FALSE);
                                                                                       glFrontFace(GL_CCW);
                                                                                   },
                                                                                   []() {
                                                                                       glFrontFace(GL_CW);
                                                                                       glDepthMask(GL_TRUE);
                                                                                       glDisable(GL_BLEND);
                                                                                   });

        pEngine->Subscribe(pParticleRenderer, ParticleRenderer::kPriority);
    }

}