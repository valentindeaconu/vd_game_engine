//
// Created by Vali on 2/23/2021.
//

#ifndef VDGE_DEPTHOFFIELDSHADER_HPP
#define VDGE_DEPTHOFFIELDSHADER_HPP

#include <engine/postprocessing/IStageShader.hpp>
#include <engine/postprocessing/MultipleInputStage.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/window/Window.hpp>

namespace mod::postprocessing {

    class DepthOfFieldShader : public vd::postprocessing::IStageShader, public vd::injector::Injectable {
    public:
        DepthOfFieldShader();

        void Link() override;

        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;

    private:
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<DepthOfFieldShader>   DepthOfFieldShaderPtr;

}

#endif //VDGE_DEPTHOFFIELDSHADER_HPP
