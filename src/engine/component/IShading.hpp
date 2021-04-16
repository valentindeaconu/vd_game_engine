//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_ISHADING_HPP
#define VD_GAME_ENGINE_ISHADING_HPP

#include <engine/gapi/gl/wrappers/Shader.hpp>

namespace vd::component {

    class IShading {
    public:
        virtual void AddUniforms(const gl::wrappers::ShaderPtr& pShader) = 0;
        virtual void SetUniforms(const gl::wrappers::ShaderPtr& pShader) = 0;
    };

}

#endif //VD_GAME_ENGINE_ISHADING_HPP
