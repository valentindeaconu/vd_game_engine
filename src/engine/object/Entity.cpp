#include "Entity.hpp"

namespace vd::object {

    math::Transform& Entity::LocalTransform() {
        return m_LocalTransform;
    }

    math::Transform& Entity::WorldTransform() {
        return m_WorldTransform;
    }

    gl::wrappers::BufferPtrVec& Entity::Buffers() {
        return m_Buffers;
    }

}
