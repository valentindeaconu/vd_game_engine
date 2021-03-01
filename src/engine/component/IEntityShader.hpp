//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_IENTITYSHADER_HPP
#define VD_GAME_ENGINE_IENTITYSHADER_HPP

#include <engine/api/gl/Shader.hpp>

#include <engine/object/Entity2D.hpp>
#include <engine/object/Entity3D.hpp>

namespace vd::component {
    class IEntityShader : public gl::Shader {
    protected:
        virtual void AddUniforms() = 0;
    public:
        virtual void InitUniforms(object::EntityPtr pEntity) = 0;
        virtual void UpdateUniforms(object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) = 0;
    };
    typedef std::shared_ptr<IEntityShader>       IEntityShaderPtr;

    class IEntity2DShader : public IEntityShader {
    public:
        void InitUniforms(object::EntityPtr pEntity) override;
        void UpdateUniforms(object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;

        virtual void InitUniforms(object::Entity2DPtr pEntity) = 0;
        virtual void UpdateUniforms(object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) = 0;
    };
    typedef std::shared_ptr<IEntity2DShader>    IEntity2DShaderPtr;

    class IEntity3DShader : public IEntityShader {
    public:
        void InitUniforms(object::EntityPtr pEntity) override;
        void UpdateUniforms(object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;

        virtual void InitUniforms(object::Entity3DPtr pEntity) = 0;
        virtual void UpdateUniforms(object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) = 0;
    };
    typedef std::shared_ptr<IEntity3DShader>    IEntity3DShaderPtr;
}

#endif //VD_GAME_ENGINE_IENTITYSHADER_HPP
