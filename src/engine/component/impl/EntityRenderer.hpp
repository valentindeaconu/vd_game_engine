#ifndef __ENTITY_RENDERER_HPP_
#define __ENTITY_RENDERER_HPP_

#include <string>

#include <engine/object/Entity.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/kernel/EngineWorker.hpp>

#include "../Renderer.hpp"

namespace vd
{
	namespace component
	{
		namespace impl
		{
			class EntityRenderer : public Renderer
			{
			public:
				EntityRenderer();
				~EntityRenderer();

				void init();
				void update();
				void cleanUp();

				vd::object::EntityPtr& getEntity();
				const vd::object::EntityPtr& getEntity() const;
				void setEntity(const vd::object::EntityPtr& entityPtr);

			private:
				bool isReady();

				vd::object::EntityPtr entityPtr;
			};
		}

		typedef impl::EntityRenderer			EntityRenderer;
		typedef std::shared_ptr<EntityRenderer>	EntityRendererPtr;
	}
}

#endif // !__REND__ENTITY_RENDERER_HPP_ERER_HPP_