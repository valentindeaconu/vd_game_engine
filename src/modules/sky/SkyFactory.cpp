//
// Created by Vali on 11/19/2020.
//

#include "SkyFactory.hpp"

namespace mod::sky
{

    void SkyFactory::Create(const vd::EnginePtr &pEngine)
    {
        vd::Consumer before = []() {
            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);
            glFrontFace(GL_CCW);
        };

        vd::Consumer after = []() {
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glFrontFace(GL_CW);
        };

        SkyPtr pSky = std::make_shared<Sky>("./resources/properties/sky.properties");
        SkyShaderPtr pSkyShader = std::make_shared<SkyShader>();

        SkyRendererPtr pSkyRenderer = std::make_shared<mod::sky::SkyRenderer>(pSky, pSkyShader, before, after);

        pEngine->Subscribe(pSkyRenderer, SkyRenderer::kDefaultPriority);

        // Required by Lens Flare
        SunPtr pSun = vd::injector::CreateAndStore<Sun>("./resources/properties/sun.properties");
        SunShaderPtr pSunShader = std::make_shared<SunShader>();

        before = []() {
            glDepthMask(GL_FALSE);
            glFrontFace(GL_CCW);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        };

        after = []() {
            glDepthMask(GL_TRUE);
            glFrontFace(GL_CW);
            glDisable(GL_BLEND);
        };

        SunRendererPtr pSunRenderer = std::make_shared<SunRenderer>(pSun, pSunShader, before, after);

        pEngine->Subscribe(pSunRenderer, SunRenderer::kDefaultPriority);

        /// Create & register rendering pass
        before = []() {
            glFrontFace(GL_CCW);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        };

        after = []() {
            glFrontFace(GL_CW);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
        };

        FlareRendererPtr pFlareRenderer = std::make_shared<FlareRenderer>("./resources/properties/flare.properties", std::make_shared<FlareShader>(), before, after);

        pEngine->Subscribe(pFlareRenderer, FlareRenderer::kPriority);
    }

}
