#include "Sky.hpp"

namespace mod
{
	namespace sky
	{
		Sky::Sky(const vd::EnginePtr& enginePtr, const mod::terrain::TerrainPtr& terrainPtr)
			: Entity(enginePtr)
			, terrainPtr(terrainPtr)
		{
		}

		Sky::~Sky()
		{
		}

		void Sky::init()
		{
			float terrainSize = (float)terrainPtr->getTerrainConfig()->getSize();
			radius = terrainSize / 2.0f;
			float terrainMinHeight = -terrainPtr->getTerrainConfig()->getMaxHeight();

			getWorldTransform().setTranslation(radius, terrainMinHeight, radius);
			getWorldTransform().setScaling(radius, radius, radius);
			vd::objloader::OBJLoaderPtr objLoaderPtr = std::make_shared<vd::objloader::OBJLoader>();

			vd::model::MeshPtrVec& meshPtrVec = getMeshes();
			objLoaderPtr->load("./resources/objects/dome", "dome.obj", meshPtrVec);

			Entity::init(); // call super.init() to initialize meshBuffers;
		}
		
		void Sky::update()
		{
			// here should be the input handler for player movement
		}

		void Sky::cleanUp()
		{
			Entity::cleanUp(); // call super.cleanUp() to clear meshBuffers;
		}

		mod::terrain::TerrainPtr& Sky::getTerrain()
		{
			return terrainPtr;
		}

		const mod::terrain::TerrainPtr& Sky::getTerrain() const
		{
			return terrainPtr;
		}

		float Sky::getRadius()
		{
			return radius;
		}

		const float Sky::getRadius() const
		{
			return radius;
		}
	}
}