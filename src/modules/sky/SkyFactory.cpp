//
// Created by Vali on 11/19/2020.
//

#include "SkyFactory.hpp"

namespace mod::sky {

    void SkyFactory::Create(const vd::EnginePtr& pEngine) {
        vd::Consumer before = []() {
            glDepthFunc(GL_LEQUAL);
            glFrontFace(GL_CCW);
        };

        vd::Consumer after = []() {
            glDepthFunc(GL_LESS);
            glFrontFace(GL_CW);
        };

        SkyPtr pSky = std::make_shared<Sky>();
        SkyShaderPtr pSkyShader = std::make_shared<SkyShader>();

        SkyRendererPtr pSkyRenderer = std::make_shared<mod::sky::SkyRenderer>(pSky, pSkyShader, before, after);

        pEngine->Subscribe(pSkyRenderer, SkyRenderer::kDefaultPriority);

        SunPtr pSun = std::make_shared<Sun>();
        SunShaderPtr pSunShader = std::make_shared<SunShader>();

        before = []() {
            glFrontFace(GL_CCW);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        };

        after = []() {
            glFrontFace(GL_CW);
            glDisable(GL_BLEND);
        };

        SunRendererPtr pSunRenderer = std::make_shared<SunRenderer>(pSun, pSunShader, before, after);

        pEngine->Subscribe(pSunRenderer, SunRenderer::kDefaultPriority);
    }

}
