#include "Entity.hpp"

namespace vd::object
{
    Entity::Entity()
        : m_Strategy(eMesh)
    {
    }

    Entity::~Entity() = default;

    void Entity::Init() {
        if (m_Buffers.empty()) {
            GenerateBuffers();
        }
    }

    void Entity::CleanUp() {
        for (auto& buffer : m_Buffers) {
            buffer->CleanUp();
        }
        m_Buffers.clear();
    }

    vd::math::Transform& Entity::GetLocalTransform() {
        return m_LocalTransform;
    }

    const vd::math::Transform& Entity::GetLocalTransform() const {
        return m_LocalTransform;
    }

    void Entity::SetLocalTransform(const vd::math::Transform& transform) {
        m_LocalTransform = transform;
    }

    vd::math::Transform& Entity::GetWorldTransform() {
        return m_WorldTransform;
    }

    const vd::math::Transform& Entity::GetWorldTransform() const {
        return m_WorldTransform;
    }

    void Entity::SetWorldTransform(const vd::math::Transform& transform) {
        m_WorldTransform = transform;
    }

    vd::model::MeshPtrVec& Entity::GetMeshes() {
        return m_Meshes;
    }

    const vd::model::MeshPtrVec& Entity::GetMeshes() const {
        return m_Meshes;
    }

    void Entity::SetMeshes(const vd::model::MeshPtrVec& meshes) {
        this->m_Meshes = meshes;

        GenerateBuffers();
    }

    vd::buffer::BufferPtrVec& Entity::GetBuffers() {
        return m_Buffers;
    }

    const vd::buffer::BufferPtrVec& Entity::GetBuffers() const {
        return m_Buffers;
    }

    vd::math::Bounds3Vec& Entity::GetBoundingBoxes() {
        return m_BoundingBoxes;
    }

    const vd::math::Bounds3Vec& Entity::GetBoundingBoxes() const {
        return m_BoundingBoxes;
    }

    void Entity::SetBoundingBoxes(const vd::math::Bounds3Vec& boundingBoxes) {
        this->m_BoundingBoxes = boundingBoxes;
    }

    void Entity::SetBufferGenerationStrategy(const BufferGenerationStrategy& strategy) {
        this->m_Strategy = strategy;
    }

    void Entity::GenerateBuffers() {
        Entity::CleanUp();

        for (auto& mesh : m_Meshes) {
            if (m_Strategy == eMesh) {
                m_Buffers.push_back(std::make_shared<vd::buffer::MeshBuffer>());
            } else {
                m_Buffers.push_back(std::make_shared<vd::buffer::PatchBuffer>());
            }
            m_Buffers.back()->Allocate(mesh);

            m_BoundingBoxes.emplace_back();
            m_BoundingBoxes.back().wrapMesh(mesh);
        }
    }
}
