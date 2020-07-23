#include "StaticObjectPlacer.hpp"

namespace mod::sobj
{
    StaticObjectPlacer::StaticObjectPlacer(const mod::sky::SkyPtr& skyPtr, size_t objectCount, float marginOffset)
        : skyPtr(skyPtr)
        , objectCount(objectCount)
        , marginOffset(marginOffset)
    {
    }

    StaticObjectPlacer::~StaticObjectPlacer() = default;

    void StaticObjectPlacer::place()
    {
        auto& terrainConfig = skyPtr->getTerrain()->getTerrainConfig();
        const mod::terrain::BiomeAtlas& biomeAtlas = terrainConfig->getBiomeAtlas();
        const size_t noOfBiomes = biomeAtlas.size();
        const size_t terrainSize = terrainConfig->getSize();
        const float skyRadius = skyPtr->getRadius();
        const float maximumDist = skyRadius - marginOffset;

        placementInfosForBiomes.resize(noOfBiomes);

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(0, terrainSize - 1);

        for (size_t objectIndex = 0; objectIndex < objectCount; ++objectIndex)
        {
            PlacementInfo placementInfo;
            size_t biomeIndex = 0;

            glm::vec3 skyCenter(skyRadius, 0.0f, skyRadius);
            float dist = skyRadius;

            do
            {
                placementInfo.location.y = 0.0f;
                do
                {
                    placementInfo.location.x = d(gen);
                    placementInfo.location.z = d(gen);

                    biomeIndex = terrainConfig->getBiomeIndex(placementInfo.location.x, placementInfo.location.z);
                } while (biomeAtlas[biomeIndex].objects.empty());

                dist = glm::length(skyCenter - placementInfo.location);
            } while (dist > maximumDist);

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

    mod::sky::SkyPtr& StaticObjectPlacer::getSky()
    {
        return skyPtr;
    }

    const mod::sky::SkyPtr& StaticObjectPlacer::getSky() const
    {
        return skyPtr;
    }

    void StaticObjectPlacer::setSky(const mod::sky::SkyPtr& skyPtr)
    {
        this->skyPtr = skyPtr;
    }
}