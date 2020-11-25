//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_ENTITY_HPP
#define VD_GAME_ENGINE_ENTITY_HPP

#include <engine/math/Transform.hpp>
#include <engine/math/Bounds.hpp>
#include <engine/model/Mesh.hpp>

#include <engine/api/gl/MeshBuffer.hpp>
#include <engine/api/gl/PatchBuffer.hpp>

namespace vd::object {
    class Entity {
    public:
        enum BufferGenerationStrategy {
            eMesh = 0,
            ePatch
        };

        Entity();
        ~Entity();

        virtual void Init();
        virtual void Update() = 0;
        virtual void CleanUp();

        math::Transform& LocalTransform();
        math::Transform& WorldTransform();

        model::MeshPtrVec& Meshes();
        gl::BufferPtrVec& Buffers();
        math::Bounds3Vec& BoundingBoxes();
    protected:
        void SetBufferGenerationStrategy(const BufferGenerationStrategy& strategy);

    private:
        void GenerateBuffers();

        math::Transform m_LocalTransform;
        math::Transform m_WorldTransform;

        BufferGenerationStrategy m_Strategy;

        model::MeshPtrVec m_Meshes;
        gl::BufferPtrVec m_Buffers;
        math::Bounds3Vec m_BoundingBoxes;
    };
    typedef std::shared_ptr<Entity>	EntityPtr;
}

#endif //VD_GAME_ENGINE_ENTITY_HPP
