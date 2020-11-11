#ifndef __ENTITY_HPP_
#define __ENTITY_HPP_

#include <engine/foundation/math/Transform.hpp>
#include <engine/foundation/math/Bounds.hpp>
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

        vd::math::Transform& GetLocalTransform();
        [[nodiscard]] const vd::math::Transform& GetLocalTransform() const;
        void SetLocalTransform(const vd::math::Transform& transform);

        vd::math::Transform& GetWorldTransform();
        [[nodiscard]] const vd::math::Transform& GetWorldTransform() const;
        void SetWorldTransform(const vd::math::Transform& transform);

        vd::model::MeshPtrVec& GetMeshes();
        [[nodiscard]] const vd::model::MeshPtrVec& GetMeshes() const;
        void SetMeshes(const vd::model::MeshPtrVec& meshes);

        vd::buffer::BufferPtrVec& GetBuffers();
        [[nodiscard]] const vd::buffer::BufferPtrVec& GetBuffers() const;

        vd::math::Bounds3Vec& GetBoundingBoxes();
        [[nodiscard]] const vd::math::Bounds3Vec& GetBoundingBoxes() const;
        void SetBoundingBoxes(const vd::math::Bounds3Vec& boundingBoxes);

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
