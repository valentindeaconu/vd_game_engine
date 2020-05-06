#ifndef __TERRAIN_HPP_
#define __TERRAIN_HPP_

#include <engine/object/Entity.hpp>

#include "TerrainConfig.hpp"

namespace mod
{
	namespace terrain
	{
		class Terrain : public vd::object::Entity
		{
		public:
			Terrain(const vd::EnginePtr& enginePtr, const std::string& configFilePath);
			~Terrain();

			void init();
			void update();
			void cleanUp();

			const TerrainConfigPtr& getTerrainConfig() const;
		private:
			void generatePatch();

			TerrainConfigPtr	configPtr;
		};
		typedef std::shared_ptr<Terrain>	TerrainPtr;

	}
}

#endif __TERRAIN_HPP_