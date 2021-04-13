//
// Created by Vali on 11/19/2020.
//

#include "SkyFactory.hpp"

namespace mod::sky {

    void SkyFactory::Create(const vd::EnginePtr& pEngine) {
        SkyPtr pSky = std::make_shared<Sky>("./resources/properties/sky.properties");
        SkyShaderPtr pSkyShader = std::make_shared<SkyShader>();

        SkyRendererPtr pSkyRenderer = std::make_shared<mod::sky::SkyRenderer>(pSky, pSkyShader);

        pEngine->Subscribe(pSkyRenderer, SkyRenderer::kDefaultPriority);

        // Required by Lens Flare
        SunPtr pSun = vd::injector::CreateAndStore<Sun>("./resources/properties/sun.properties");
        SunShaderPtr pSunShader = std::make_shared<SunShader>();

        SunRendererPtr pSunRenderer = std::make_shared<SunRenderer>(pSun, pSunShader);

        pEngine->Subscribe(pSunRenderer, SunRenderer::kDefaultPriority);

        FlareRendererPtr pFlareRenderer = std::make_shared<FlareRenderer>("./resources/properties/flare.properties");

        pEngine->Subscribe(pFlareRenderer, FlareRenderer::kPriority);
    }

}
