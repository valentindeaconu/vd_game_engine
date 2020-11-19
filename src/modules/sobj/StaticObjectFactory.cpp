//
// Created by Vali on 11/19/2020.
//

#include "StaticObjectFactory.hpp"

namespace mod::sobj {

    void mod::sobj::StaticObjectFactory::Create(const vd::EnginePtr& pEngine) {
        StaticObjectPlacerPtr staticObjectPlacerPtr = std::make_shared<StaticObjectPlacer>(7000, 10.0f);
        StaticObjectShaderPtr staticObjectShaderPtr = std::make_shared<StaticObjectShader>();

        StaticObjectRendererPtr staticObjectRendererPtr = std::make_shared<StaticObjectRenderer>(staticObjectPlacerPtr,
                                                                                                 staticObjectShaderPtr,
                                                                                                 []() { glFrontFace(GL_CCW); },
                                                                                                 []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(staticObjectRendererPtr, StaticObjectRenderer::kDefaultPriority);
    }

}
