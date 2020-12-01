//
// Created by Vali on 11/19/2020.
//

#include "PropsFactory.hpp"

namespace mod::props {

    void mod::props::PropsFactory::Create(const vd::EnginePtr& pEngine) {
        PropShaderPtr pPropShader = std::make_shared<PropShader>();

        PropsRendererPtr pPropsRenderer = std::make_shared<PropsRenderer>(pPropShader,
                                                                          []() { glFrontFace(GL_CCW); },
                                                                          []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(pPropsRenderer, PropsRenderer::kDefaultPriority);
    }

}
