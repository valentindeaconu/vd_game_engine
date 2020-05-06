#include "TerrainRenderer.hpp"

namespace mod
{
	namespace terrain
	{
		TerrainRenderer::TerrainRenderer()
			: Renderer()
			, terrainPtr(nullptr)
		{
		}

		TerrainRenderer::~TerrainRenderer()
		{
		}

		void TerrainRenderer::init()
		{
			terrainPtr->init();
		}

		void TerrainRenderer::update()
		{
			if (isReady())
			{
				terrainPtr->update();

				if (renderConfigPtr != nullptr)
				{
					renderConfigPtr->enable();
				}

				shaderPtr->bind();

				//const size_t patchSize = terrainPtr->getTerrainConfig()->getPatchSize();
				//for (size_t i = 0; i < terrainPtr->getGridY(); ++i)
				//{
					//for (size_t j = 0; j < terrainPtr->getGridX(); ++j)
					//{
						//terrainPtr->getLocalTransform().setTranslation(i * patchSize, 0.0f, j * patchSize);
						shaderPtr->updateUniforms(terrainPtr, 0);
						terrainPtr->getMeshBuffers()[0]->render();
					//}
				//}

				if (renderConfigPtr != nullptr)
				{
					renderConfigPtr->disable();
				}
			}
		}

		void TerrainRenderer::cleanUp()
		{
			terrainPtr->cleanUp();
		}

		TerrainPtr& TerrainRenderer::getTerrain()
		{
			return terrainPtr;
		}

		const TerrainPtr& TerrainRenderer::getTerrain() const
		{
			return terrainPtr;
		}

		void TerrainRenderer::setTerrain(const TerrainPtr& terrainPtr)
		{
			this->terrainPtr = terrainPtr;
		}

		bool TerrainRenderer::isReady()
		{
			return Renderer::isReady() && terrainPtr != nullptr;
		}
	}
}