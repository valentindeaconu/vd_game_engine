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

    math::Transform& Entity::LocalTransform() {
        return m_LocalTransform;
    }

    math::Transform& Entity::WorldTransform() {
        return m_WorldTransform;
    }

    model::MeshPtrVec& Entity::Meshes() {
        return m_Meshes;
    }

    gl::BufferPtrVec& Entity::Buffers() {
        return m_Buffers;
    }

    math::Bounds3Vec& Entity::BoundingBoxes() {
        return m_BoundingBoxes;
    }

    void Entity::SetBufferGenerationStrategy(const BufferGenerationStrategy& strategy) {
        this->m_Strategy = strategy;
    }

    void Entity::GenerateBuffers() {
        Entity::CleanUp();

        for (auto& mesh : m_Meshes) {
            if (m_Strategy == eMesh) {
                m_Buffers.push_back(std::make_shared<gl::MeshBuffer>());
            } else {
                m_Buffers.push_back(std::make_shared<gl::PatchBuffer>());
            }
            m_Buffers.back()->Allocate(mesh);

            m_BoundingBoxes.emplace_back();
            m_BoundingBoxes.back().WrapMesh(mesh);
        }
    }
}
