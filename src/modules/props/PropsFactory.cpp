//
// Created by Vali on 11/19/2020.
//

#include "PropsFactory.hpp"

namespace mod::props {

    void mod::props::PropsFactory::Create(const vd::EnginePtr& pEngine) {
        PropShaderPtr pPropShader = std::make_shared<PropShader>();
        PropsManagerPtr pPropsManager = std::make_shared<PropsManager>("./resources/properties/props.properties");

        PropsRendererPtr pPropsRenderer = std::make_shared<PropsRenderer>(pPropsManager,
                                                                          pPropShader,
                                                                          []() { glFrontFace(GL_CCW); },
                                                                          []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(pPropsRenderer, PropsRenderer::kDefaultPriority);
    }

}
