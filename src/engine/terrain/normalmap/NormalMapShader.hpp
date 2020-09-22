//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_NORMALMAPSHADER_HPP
#define VD_GAME_ENGINE_NORMALMAPSHADER_HPP

#include <engine/shader/Shader.hpp>

namespace vd::terrain::normalmap {
    class NormalMapShader : public shader::Shader {
    public:
        NormalMapShader();
        ~NormalMapShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    };
    typedef std::shared_ptr<NormalMapShader>    NormalMapShaderPtr;
}


#endif //VD_GAME_ENGINE_NORMALMAPSHADER_HPP
