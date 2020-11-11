//
// Created by Vali on 8/26/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWSHADER_HPP
#define VD_GAME_ENGINE_SHADOWSHADER_HPP

#include <engine/shader/Shader.hpp>

#include <engine/shadow/ShadowManager.hpp>
#include <engine/core/ObjectOfType.hpp>

#include <memory>

namespace vd::shadow {
    class ShadowShader : public vd::shader::Shader {
    public:
        ShadowShader();
        ~ShadowShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    private:
    };
    typedef std::shared_ptr<ShadowShader> ShadowShaderPtr;

    ShadowShaderPtr& GetShadowShader();
}


#endif //VD_GAME_ENGINE_SHADOWSHADER_HPP
