#include "StaticObjectRenderer.hpp"

namespace mod
{
	namespace sobj
	{
		StaticObjectRenderer::StaticObjectRenderer()
			: Renderer()
			, staticObjectPlacerPtr(nullptr)
		{
		}

		StaticObjectRenderer::~StaticObjectRenderer()
		{
		}

		void StaticObjectRenderer::init()
		{
			if (isReady())
			{
				staticObjectPlacerPtr->place();
			}
		}

		void StaticObjectRenderer::update()
		{
			if (isReady())
			{
				if (renderConfigPtr != nullptr)
				{
					renderConfigPtr->enable();
				}

				shaderPtr->bind();

				const PlacementInfoMat& placementInfosForBiomes = staticObjectPlacerPtr->getPlacementInfosForBiomes();
				const terrain::BiomeAtlas& biomeAtlas = staticObjectPlacerPtr->getSky()->getTerrain()->getTerrainConfig()->getBiomeAtlas();

				for (size_t biomeIndex = 0; biomeIndex < biomeAtlas.size(); ++biomeIndex)
				{
					const terrain::Biome& biome = biomeAtlas[biomeIndex];
					const PlacementInfoVec& placementInfos = placementInfosForBiomes[biomeIndex];

					if (!placementInfos.empty())
					{
						for (auto it = placementInfos.begin(); it != placementInfos.end(); ++it)
						{
							StaticObjectPtr staticObjectPtr = biome.objects[it->objectIndex];
							staticObjectPtr->getWorldTransform().setTranslation(it->location);
							staticObjectPtr->update();

							for (size_t meshIndex = 0; meshIndex < staticObjectPtr->getMeshBuffers().size(); ++meshIndex)
							{
								shaderPtr->updateUniforms(staticObjectPtr, meshIndex);
								staticObjectPtr->getMeshBuffers()[meshIndex]->render();
							}
						}
					}
				}

				if (renderConfigPtr != nullptr)
				{
					renderConfigPtr->disable();
				}
			}
		}
		
		void StaticObjectRenderer::cleanUp()
		{
			const terrain::BiomeAtlas& biomeAtlas = staticObjectPlacerPtr->getSky()->getTerrain()->getTerrainConfig()->getBiomeAtlas();
			for (size_t biomeIndex = 0; biomeIndex < biomeAtlas.size(); ++biomeIndex)
			{
				const terrain::Biome& biome = biomeAtlas[biomeIndex];

				if (!biome.objects.empty())
				{
					for (auto it = biome.objects.begin(); it != biome.objects.end(); ++it)
					{
						(*it)->cleanUp();
					}
				}
			}
		}

		StaticObjectPlacerPtr& StaticObjectRenderer::getStaticObjectPlacer()
		{
			return staticObjectPlacerPtr;
		}

		const StaticObjectPlacerPtr& StaticObjectRenderer::getStaticObjectPlacer() const
		{
			return staticObjectPlacerPtr;
		}

		void StaticObjectRenderer::setStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr)
		{
			this->staticObjectPlacerPtr = staticObjectPlacerPtr;
		}

		bool StaticObjectRenderer::isReady()
		{
			return Renderer::isReady() && staticObjectPlacerPtr != nullptr;
		}
	}
}