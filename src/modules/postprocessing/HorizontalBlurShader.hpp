//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_HORIZONTALBLURSHADER_HPP
#define VDGE_HORIZONTALBLURSHADER_HPP

#include <engine/postprocessing/IStageShader.hpp>
#include <engine/postprocessing/SingularInputStage.hpp>

namespace mod::postprocessing {
    class HorizontalBlurShader : public vd::postprocessing::IStageShader {
    public:
        HorizontalBlurShader();

        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<HorizontalBlurShader>   HorizontalBlurShaderPtr;
}

#endif //VDGE_HORIZONTALBLURSHADER_HPP
