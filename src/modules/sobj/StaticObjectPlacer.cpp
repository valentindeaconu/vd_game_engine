#include "StaticObjectPlacer.hpp"

namespace mod::sobj
{
    StaticObjectPlacer::StaticObjectPlacer(const mod::terrain::TerrainPtr& terrainPtr, size_t objectCount, float marginOffset)
        : terrainPtr(terrainPtr)
        , objectCount(objectCount)
        , marginOffset(marginOffset)
    {
    }

    StaticObjectPlacer::~StaticObjectPlacer() = default;

    void StaticObjectPlacer::place()
    {
        auto& terrainConfig = terrainPtr->getTerrainConfig();
        const mod::terrain::BiomeAtlas& biomeAtlas = terrainConfig->getBiomeAtlas();
        const size_t noOfBiomes = biomeAtlas.size();
        const size_t terrainSize = terrainConfig->getSize();

        placementInfosForBiomes.resize(noOfBiomes);

        auto onSurface = [&terrainSize](const float x, const float z) -> bool {
            return (x >= 0.0f && z >= 0.0f && x <= terrainSize && z <= terrainSize);
        };

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(0, terrainSize - 1);

        for (size_t objectIndex = 0; objectIndex < objectCount; ++objectIndex)
        {
            PlacementInfo placementInfo;
            size_t biomeIndex = 0;

            do
            {
                placementInfo.location.y = 0.0f;
                do
                {
                    placementInfo.location.x = d(gen);
                    placementInfo.location.z = d(gen);

                    biomeIndex = terrainConfig->getBiomeIndex(placementInfo.location.x, placementInfo.location.z);
                } while (biomeAtlas[biomeIndex].objects.empty());

            } while (!onSurface(placementInfo.location.x, placementInfo.location.z));

            placementInfo.location.y = terrainConfig->getHeight(placementInfo.location.x, placementInfo.location.z);

            std::uniform_int_distribution<size_t> d_i(0, biomeAtlas[biomeIndex].objects.size() - 1);
            placementInfo.objectIndex = d_i(gen);

            placementInfosForBiomes[biomeIndex].push_back(placementInfo);
        }
    }

    const PlacementInfoMat& StaticObjectPlacer::getPlacementInfosForBiomes() const
    {
        return placementInfosForBiomes;
    }

    terrain::TerrainPtr &StaticObjectPlacer::getTerrain() {
        return terrainPtr;
    }

    const terrain::TerrainPtr &StaticObjectPlacer::getTerrain() const {
        return terrainPtr;
    }
}