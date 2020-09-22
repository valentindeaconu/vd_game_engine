#ifndef __ENTITY_HPP_
#define __ENTITY_HPP_

#include <glm/glm.hpp>

#include <engine/foundation/math/Transform.hpp>
#include <engine/foundation/math/BoundingBox.hpp>
#include <engine/model/Mesh.hpp>

#include <engine/glmodel/buffer/MeshBuffer.hpp>
#include <engine/glmodel/buffer/PatchBuffer.hpp>

#include <unordered_map>
#include <algorithm>

#include <engine/kernel/EngineWorker.hpp>
#include <engine/kernel/EngineBlock.hpp>

namespace vd::object {
    class Entity {
    public:
        enum BufferGenerationStrategy {
            eMesh = 0,
            ePatch
        };

        explicit Entity(const vd::EnginePtr& enginePtr);
        ~Entity();

        virtual void init();
        virtual void update() = 0;
        virtual void cleanUp();

        [[nodiscard]] bool shouldBeRendered() const;

        vd::math::Transform& getLocalTransform();
        [[nodiscard]] const vd::math::Transform& getLocalTransform() const;
        void getLocalTransform(const vd::math::Transform& transform);

        vd::math::Transform& getWorldTransform();
        [[nodiscard]] const vd::math::Transform& getWorldTransform() const;
        void getWorldTransform(const vd::math::Transform& transform);

        vd::model::MeshPtrVec& getMeshes();
        [[nodiscard]] const vd::model::MeshPtrVec& getMeshes() const;
        void setMeshes(const vd::model::MeshPtrVec& meshes);

        vd::buffer::BufferPtrVec& getBuffers();
        [[nodiscard]] const vd::buffer::BufferPtrVec& getBuffers() const;

        vd::math::BoundingBoxVec& getBoundingBoxes();
        [[nodiscard]] const vd::math::BoundingBoxVec& getBoundingBoxes() const;
        void setBoundingBoxes(const vd::math::BoundingBoxVec& boundingBoxes);

        vd::EnginePtr& getParentEngine();
        [[nodiscard]] const vd::EnginePtr& getParentEngine() const;
        void setParentEngine(const vd::EnginePtr& enginePtr);

    protected:
        void setBufferGenerationStrategy(const BufferGenerationStrategy& strategy);

    private:
        void generateBuffers();

        vd::math::Transform localTransform;
        vd::math::Transform worldTransform;

        BufferGenerationStrategy strategy;

        vd::model::MeshPtrVec meshes;
        vd::buffer::BufferPtrVec buffers;
        vd::math::BoundingBoxVec boundingBoxes;

        vd::EnginePtr parentEnginePtr;
    };
    typedef std::shared_ptr<Entity>	EntityPtr;
}

#endif // !__ENTITY_HPP_
