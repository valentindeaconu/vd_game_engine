#include "EntityRenderer.hpp"

namespace vd
{
	namespace component
	{
		namespace impl
		{
			EntityRenderer::EntityRenderer()
				: Renderer()
				, entityPtr(nullptr)
			{
			}

			EntityRenderer::~EntityRenderer()
			{
			}

			void EntityRenderer::init()
			{
				entityPtr->init();
			}

			void EntityRenderer::update()
			{
				if (isReady())
				{
					entityPtr->update();

					if (renderConfigPtr != nullptr)
					{
						renderConfigPtr->enable();
					}

					shaderPtr->bind();
					buffer::MeshBufferPtrVec& meshBuffers = entityPtr->getMeshBuffers();
					for (size_t meshIndex = 0; meshIndex < meshBuffers.size(); ++meshIndex)
					{
						shaderPtr->updateUniforms(entityPtr, meshIndex);
						meshBuffers[meshIndex]->render();
					}

					if (renderConfigPtr != nullptr)
					{
						renderConfigPtr->disable();
					}
				}
			}

			void EntityRenderer::cleanUp()
			{
				entityPtr->cleanUp();
			}

			vd::object::EntityPtr& EntityRenderer::getEntity()
			{
				return entityPtr;
			}

			const vd::object::EntityPtr& EntityRenderer::getEntity() const
			{
				return entityPtr;
			}

			void EntityRenderer::setEntity(const vd::object::EntityPtr& entityPtr)
			{
				this->entityPtr = entityPtr;
			}

			bool EntityRenderer::isReady()
			{
				return Renderer::isReady() && entityPtr != nullptr;
			}
		}
	}
}