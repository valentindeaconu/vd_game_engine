//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_ISTAGESHADER_HPP
#define VDGE_ISTAGESHADER_HPP

#include <memory>

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include "Stage.hpp"

namespace vd::postprocessing {
    class IStageShader : public gl::Shader {
    protected:
        virtual void AddUniforms() = 0;
    public:
        virtual void InitUniforms(StagePtr pStage) = 0;
        virtual void UpdateUniforms(StagePtr pStage) = 0;
    };
    typedef std::shared_ptr<IStageShader>    IStageShaderPtr;
}


#endif //VDGE_ISTAGESHADER_HPP
