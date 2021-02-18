//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_ENTITY_HPP
#define VD_GAME_ENGINE_ENTITY_HPP

#include <engine/math/Transform.hpp>
#include <engine/api/gl/Buffer.hpp>

namespace vd::object {
    class Entity {
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void CleanUp() = 0;

        math::Transform& LocalTransform();
        math::Transform& WorldTransform();

        gl::BufferPtrVec& Buffers();
    private:
        math::Transform m_LocalTransform;
        math::Transform m_WorldTransform;

        gl::BufferPtrVec m_Buffers;
    };
    typedef std::shared_ptr<Entity>	EntityPtr;
}

#endif //VD_GAME_ENGINE_ENTITY_HPP
