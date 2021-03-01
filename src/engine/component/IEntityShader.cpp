//
// Created by Vali on 2/17/2021.
//

#include "IEntityShader.hpp"

namespace vd::component {

    void IEntity2DShader::InitUniforms(object::EntityPtr pEntity) {
        object::Entity2DPtr p = std::dynamic_pointer_cast<object::Entity2D>(pEntity);
        InitUniforms(p);
    }

    void IEntity2DShader::UpdateUniforms(object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        object::Entity2DPtr p = std::dynamic_pointer_cast<object::Entity2D>(pEntity);
        UpdateUniforms(p, levelOfDetail, meshIndex);
    }

    void IEntity3DShader::InitUniforms(object::EntityPtr pEntity) {
        object::Entity3DPtr p = std::dynamic_pointer_cast<object::Entity3D>(pEntity);
        InitUniforms(p);
    }

    void IEntity3DShader::UpdateUniforms(object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        object::Entity3DPtr p = std::dynamic_pointer_cast<object::Entity3D>(pEntity);
        UpdateUniforms(p, levelOfDetail, meshIndex);
    }
}
