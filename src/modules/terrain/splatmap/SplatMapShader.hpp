//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_SPLATMAPSHADER_HPP
#define VD_GAME_ENGINE_SPLATMAPSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

namespace mod::terrain::splatmap {
    class SplatMapShader : public vd::gl::Shader {
    public:
        SplatMapShader();
        ~SplatMapShader();

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<SplatMapShader>    SplatMapShaderPtr;
}


#endif //VD_GAME_ENGINE_SPLATMAPSHADER_HPP
