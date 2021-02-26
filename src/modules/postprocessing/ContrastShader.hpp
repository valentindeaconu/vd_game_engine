//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_CONTRASTSHADER_HPP
#define VDGE_CONTRASTSHADER_HPP

#include <engine/postprocessing/SingularInputStage.hpp>
#include <engine/postprocessing/IStageShader.hpp>

namespace mod::postprocessing {
    class ContrastShader : public vd::postprocessing::IStageShader {
    public:
        ContrastShader();
        void InitUniforms(vd::postprocessing::StagePtr pStage) override;
        void UpdateUniforms(vd::postprocessing::StagePtr pStage) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<ContrastShader>   ContrastShaderPtr;

}


#endif //VDGE_CONTRASTSHADER_HPP
