#ifndef __ENTITY_HPP_
#define __ENTITY_HPP_

#include <glm/glm.hpp>

#include <engine/foundation/math/Transform.hpp>
#include <engine/model/Mesh.hpp>
#include <engine/glmodel/buffer/MeshBuffer.hpp>

#include <unordered_map>

#include <engine/kernel/EngineWorker.hpp>
#include <engine/kernel/EngineBlock.hpp>

namespace vd
{
	namespace object
	{
		class Entity
		{
		public:
			Entity(const vd::EnginePtr& enginePtr);
			~Entity();

			virtual void init();
			virtual void update() = 0;
			virtual void cleanUp();

			vd::math::Transform& getLocalTransform();
			const vd::math::Transform& getLocalTransform() const;
			void getLocalTransform(const vd::math::Transform& transform);

			vd::math::Transform& getWorldTransform();
			const vd::math::Transform& getWorldTransform() const;
			void getWorldTransform(const vd::math::Transform& transform);

			vd::model::MeshPtrVec& getMeshes();
			const vd::model::MeshPtrVec& getMeshes() const;
			void setMeshes(const vd::model::MeshPtrVec& meshes);

			vd::buffer::MeshBufferPtrVec& getMeshBuffers();
			const vd::buffer::MeshBufferPtrVec& getMeshBuffers() const;

			vd::EnginePtr& getParentEngine();
			const vd::EnginePtr& getParentEngine() const;
			void setParentEngine(const vd::EnginePtr& enginePtr);
		private:
			void generateBuffers();

			vd::math::Transform localTransform;
			vd::math::Transform worldTransform;

			vd::model::MeshPtrVec meshes;
			vd::buffer::MeshBufferPtrVec meshBuffers;

			vd::EnginePtr parentEnginePtr;
		};
		typedef std::shared_ptr<Entity>	EntityPtr;
	}
}

#endif // !__ENTITY_HPP_
