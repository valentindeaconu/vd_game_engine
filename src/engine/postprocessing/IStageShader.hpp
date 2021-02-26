//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_STAGESHADER_HPP
#define VDGE_STAGESHADER_HPP

#include <memory>

#include <engine/api/gl/Shader.hpp>

#include "Stage.hpp"

namespace vd::postprocessing {
    class IStageShader : public gl::Shader {
    protected:
        virtual void AddUniforms() = 0;
    public:
        virtual void InitUniforms(StagePtr pStage) = 0;
        virtual void UpdateUniforms(StagePtr pStage) = 0;
    };
    typedef std::shared_ptr<StageShader>    StageShaderPtr;
}


#endif //VDGE_STAGESHADER_HPP
