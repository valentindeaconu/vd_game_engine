//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_IRENDERINGEFFECTSHADER_HPP
#define VDGE_IRENDERINGEFFECTSHADER_HPP

#include <memory>

#include <engine/gapi/gl/wrappers/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include "RenderingEffect.hpp"

namespace vd::component {
    class IRenderingEffectShader : public gl::wrappers::Shader {
    protected:
        virtual void AddUniforms() = 0;
    public:
        virtual void InitUniforms(IRenderingEffectPtr pEffect) = 0;
        virtual void UpdateUniforms(IRenderingEffectPtr pEffect) = 0;
    };
    typedef std::shared_ptr<IRenderingEffectShader>    IRenderingEffectShaderPtr;
}


#endif //VDGE_IRENDERINGEFFECTSHADER_HPP
