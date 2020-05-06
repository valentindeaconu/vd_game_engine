#ifndef __STATIC_OBJECT_RENDERER_HPP_
#define __STATIC_OBJECT_RENDERER_HPP_

#include <engine/component/Renderer.hpp>
#include <engine/shader/Shader.hpp>

#include <modules/terrain/Terrain.hpp>

#include "StaticObjectPlacer.hpp"

namespace mod
{
	namespace sobj
	{
		class StaticObjectRenderer : public vd::component::Renderer
		{
		public:
			StaticObjectRenderer();
			~StaticObjectRenderer();

			void init();
			void update();
			void cleanUp();

			StaticObjectPlacerPtr& getStaticObjectPlacer();
			const StaticObjectPlacerPtr& getStaticObjectPlacer() const;
			void setStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr);

		private:
			bool isReady();

			StaticObjectPlacerPtr staticObjectPlacerPtr;
		};
		typedef std::shared_ptr<StaticObjectRenderer>	StaticObjectRendererPtr;
	}
}

#endif // !__STATIC_OBJECT_RENDERER_HPP_
