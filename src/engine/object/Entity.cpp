#include "Entity.hpp"

namespace vd::object
{
    Entity::Entity(const vd::EnginePtr& enginePtr)
        : parentEnginePtr(enginePtr)
    {
    }

    Entity::~Entity() = default;

    void Entity::init()
    {
        if (meshBuffers.empty())
        {
            generateBuffers();
        }
    }

    void Entity::cleanUp()
    {
        for (auto& meshBuffer : meshBuffers)
        {
            meshBuffer->cleanUp();
        }
        meshBuffers.clear();
    }

    bool Entity::shouldBeRendered() const {
        return std::ranges::any_of(boundingBoxes.cbegin(),
                                   boundingBoxes.cend(),
                                   [&](const math::BoundingBox& boundingBox) {
            return parentEnginePtr
                ->getFrustum()
                ->checkAgainst(boundingBox.withTransform(worldTransform)) != math::Frustum::eOutside;
        });
    }

    vd::math::Transform& Entity::getLocalTransform()
    {
        return localTransform;
    }

    const vd::math::Transform& Entity::getLocalTransform() const
    {
        return localTransform;
    }

    void Entity::getLocalTransform(const vd::math::Transform& transform)
    {
        localTransform = transform;
    }

    vd::math::Transform& Entity::getWorldTransform()
    {
        return worldTransform;
    }

    const vd::math::Transform& Entity::getWorldTransform() const
    {
        return worldTransform;
    }

    void Entity::getWorldTransform(const vd::math::Transform& transform)
    {
        worldTransform = transform;
    }

    vd::model::MeshPtrVec& Entity::getMeshes()
    {
        return meshes;
    }

    const vd::model::MeshPtrVec& Entity::getMeshes() const
    {
        return meshes;
    }

    void Entity::setMeshes(const vd::model::MeshPtrVec& meshes)
    {
        this->meshes = meshes;

        generateBuffers();
    }

    vd::buffer::MeshBufferPtrVec& Entity::getMeshBuffers()
    {
        return meshBuffers;
    }

    const vd::buffer::MeshBufferPtrVec& Entity::getMeshBuffers() const
    {
        return meshBuffers;
    }

    vd::math::BoundingBoxVec& Entity::getBoundingBoxes() {
        return boundingBoxes;
    }

    const vd::math::BoundingBoxVec& Entity::getBoundingBoxes() const {
        return boundingBoxes;
    }

    void Entity::setBoundingBoxes(const vd::math::BoundingBoxVec& boundingBoxes) {
        this->boundingBoxes = boundingBoxes;
    }

    vd::EnginePtr& Entity::getParentEngine()
    {
        return parentEnginePtr;
    }

    const vd::EnginePtr& Entity::getParentEngine() const
    {
        return parentEnginePtr;
    }

    void Entity::setParentEngine(const vd::EnginePtr& enginePtr)
    {
        this->parentEnginePtr = enginePtr;
    }

    void Entity::generateBuffers()
    {
        Entity::cleanUp();

        for (auto& mesh : meshes)
        {
            meshBuffers.push_back(std::make_shared<vd::buffer::MeshBuffer>());
            meshBuffers.back()->allocate(mesh);

            boundingBoxes.emplace_back();
            boundingBoxes.back().wrapMesh(mesh);
        }
    }
}
