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
        auto& terrainConfig = terrainPtr->GetTerrainConfig();
        const mod::terrain::BiomePtrVec& biomeAtlas = terrainConfig->getBiomes();
        const size_t noOfBiomes = biomeAtlas.size();
        const auto terrainSize = size_t(terrainConfig->getScaleXZ());

        placementInfosForBiomes.resize(noOfBiomes);

        auto onSurface = [&terrainSize](const float x, const float z) -> bool {
            return (x >= 0.0f && z >= 0.0f && x <= terrainSize && z <= terrainSize);
        };

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(0, terrainSize - 1);

        // TODO: Adapt to the new terrain
        for (size_t objectIndex = 0; objectIndex < objectCount; ++objectIndex)
        {
            /*PlacementInfo placementInfo;
            size_t biomeIndex = 0;

            do
            {
                placementInfo.location.y = 0.0f;
                do
                {
                    placementInfo.location.x = d(gen);
                    placementInfo.location.z = d(gen);

                    biomeIndex = terrainConfig->getBiome(placementInfo.location.x, placementInfo.location.z);
                } while (biomeAtlas[biomeIndex].objects.empty());

            } while (!onSurface(placementInfo.location.x, placementInfo.location.z));

            placementInfo.location.y = terrainConfig->getHeight(placementInfo.location.x, placementInfo.location.z);

            std::uniform_int_distribution<size_t> d_i(0, biomeAtlas[biomeIndex].objects.size() - 1);
            placementInfo.objectIndex = d_i(gen);

            placementInfosForBiomes[biomeIndex].push_back(placementInfo);*/
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