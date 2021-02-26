//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_VERTICALBLURSHADER_HPP
#define VDGE_VERTICALBLURSHADER_HPP

#include <engine/postprocessing/IStageShader.hpp>
#include <engine/postprocessing/SingularInputStage.hpp>

namespace mod::postprocessing {
    class VerticalBlurShader : public vd::postprocessing::IStageShader {
    public:
        VerticalBlurShader();

        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<VerticalBlurShader>   VerticalBlurShaderPtr;
}


#endif //VDGE_VERTICALBLURSHADER_HPP
