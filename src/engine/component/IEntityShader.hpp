//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_IENTITYSHADER_HPP
#define VD_GAME_ENGINE_IENTITYSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/object/Entity.hpp>

namespace vd::component {

    class IEntityShader : public gl::Shader {
    protected:
        virtual void AddUniforms() = 0;

    public:
        virtual void InitUniforms(object::EntityPtr pEntity) = 0;
        virtual void UpdateUniforms(object::EntityPtr pEntity, uint32_t meshIndex) = 0;
    };
    typedef std::shared_ptr<IEntityShader> EntityShaderPtr;

}

#endif //VD_GAME_ENGINE_IENTITYSHADER_HPP
