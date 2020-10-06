//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_SPLATMAPSHADER_HPP
#define VD_GAME_ENGINE_SPLATMAPSHADER_HPP

#include <engine/shader/Shader.hpp>

namespace mod::terrain::splatmap {
    class SplatMapShader : public vd::shader::Shader {
    public:
        SplatMapShader();
        ~SplatMapShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    };
    typedef std::shared_ptr<SplatMapShader>    SplatMapShaderPtr;
}


#endif //VD_GAME_ENGINE_SPLATMAPSHADER_HPP
