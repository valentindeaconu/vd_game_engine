//
// Created by Vali on 11/19/2020.
//

#include "ShadowFactory.hpp"

namespace mod::shadow {

    void ShadowFactory::Create(const vd::EnginePtr& pEngine) {
        ShadowManagerPtr pShadowManager =
                vd::injector::CreateAndStore<mod::shadow::ShadowManager>("./resources/properties/shadow.properties");

        pEngine->Subscribe(pShadowManager, ShadowManager::kDefaultPriority);

        vd::component::RenderingPass shadowRenderingPass(
                "Shadow",
                10,
                pShadowManager->FrameBuffer(),
                true,
                vd::g_kEmptyPredicate,
                []() { glDisable(GL_CULL_FACE); },
                []() { glEnable(GL_CULL_FACE); }
        );

        pEngine->Add(shadowRenderingPass);

        ShadowShaderPtr pShadowShader = vd::injector::CreateAndStore<mod::shadow::ShadowShader>();

        pShadowShader->InitUniforms(nullptr);
    }

}