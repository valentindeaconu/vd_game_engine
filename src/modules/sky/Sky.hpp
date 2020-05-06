#ifndef __SKY_HPP_
#define __SKY_HPP_

#include <engine/object/Entity.hpp>
#include <engine/foundation/objloader/OBJLoader.hpp>

#include <modules/terrain/Terrain.hpp>

namespace mod
{
	namespace sky
	{
		class Sky : public vd::object::Entity
		{
		public:
			Sky(const vd::EnginePtr& enginePtr, const mod::terrain::TerrainPtr& terrainPtr);
			~Sky();

			void init();
			void update();
			void cleanUp();

			mod::terrain::TerrainPtr& getTerrain();
			const mod::terrain::TerrainPtr& getTerrain() const;

			float getRadius();
			const float getRadius() const;
		private:
			float radius;

			mod::terrain::TerrainPtr terrainPtr;
		};
		typedef std::shared_ptr<Sky>	SkyPtr;
	}
}

#endif // !__SKY_HPP_
