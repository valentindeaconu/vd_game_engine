//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_POSTPROCESSINGFACTORY_HPP
#define VDGE_POSTPROCESSINGFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/postprocessing/SingularInputStage.hpp>
#include <engine/postprocessing/MultipleInputStage.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/window/Window.hpp>
#include <engine/kernel/Context.hpp>

#include "ContrastShader.hpp"
#include "DepthOfFieldShader.hpp"
#include "HorizontalBlurShader.hpp"
#include "VerticalBlurShader.hpp"

namespace mod::postprocessing {
    class PostProcessingFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
}


#endif //VDGE_POSTPROCESSINGFACTORY_HPP
