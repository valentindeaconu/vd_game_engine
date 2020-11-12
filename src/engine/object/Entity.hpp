#ifndef __ENTITY_HPP_
#define __ENTITY_HPP_

#include <engine/math/Transform.hpp>
#include <engine/math/Bounds.hpp>
#include <engine/model/Mesh.hpp>

#include <engine/glmodel/buffer/MeshBuffer.hpp>
#include <engine/glmodel/buffer/PatchBuffer.hpp>

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

        vd::math::Transform& LocalTransform();

        vd::math::Transform& WorldTransform();

        vd::model::MeshPtrVec& Meshes();

        vd::buffer::BufferPtrVec& Buffers();

        vd::math::Bounds3Vec& BoundingBoxes();
    protected:
        void SetBufferGenerationStrategy(const BufferGenerationStrategy& strategy);

    private:
        void GenerateBuffers();

        vd::math::Transform m_LocalTransform;
        vd::math::Transform m_WorldTransform;

        BufferGenerationStrategy m_Strategy;

        vd::model::MeshPtrVec m_Meshes;
        vd::buffer::BufferPtrVec m_Buffers;
        vd::math::Bounds3Vec m_BoundingBoxes;
    };
    typedef std::shared_ptr<Entity>	EntityPtr;
}

#endif // !__ENTITY_HPP_
