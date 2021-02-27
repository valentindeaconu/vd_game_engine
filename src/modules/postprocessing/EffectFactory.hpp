//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_EFFECTFACTORY_HPP
#define VDGE_EFFECTFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/window/Window.hpp>
#include <engine/kernel/Context.hpp>

#include <modules/postprocessing/effects/ContrastShader.hpp>
#include <modules/postprocessing/effects/HorizontalBlurShader.hpp>
#include <modules/postprocessing/effects/VerticalBlurShader.hpp>
#include <modules/postprocessing/effects/DepthOfFieldShader.hpp>

#include "EffectRenderer.hpp"
#include "ToScreenPseudoEffect.hpp"

namespace mod::postprocessing {
    class EffectFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
}


#endif //VDGE_EFFECTFACTORY_HPP
