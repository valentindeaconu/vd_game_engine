#include "StaticObjectPlacer.hpp"

namespace mod::sobj
{
    StaticObjectPlacer::StaticObjectPlacer(size_t objectCount, float marginOffset)
        : objectCount(objectCount)
        , marginOffset(marginOffset)
    {
        terrainPtr = vd::ObjectOfType<mod::terrain::Terrain>::Find();
    }

    StaticObjectPlacer::~StaticObjectPlacer() = default;

    void StaticObjectPlacer::place()
    {
        const auto scaleXZ = terrainPtr->GetProperties()->Get<float>("ScaleXZ");

        const mod::terrain::BiomePtrVec& biomeAtlas = terrainPtr->GetBiomes();
        const auto terrainSize = int(scaleXZ / 2.0f);

        auto onSurface = [&terrainSize](const float x, const float z) -> bool {
            return (x > -terrainSize && z > -terrainSize && x < terrainSize && z < terrainSize);
        };

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(-terrainSize, terrainSize);

        for (size_t objectIndex = 0; objectIndex < objectCount; ++objectIndex)
        {
            PlacementInfo placementInfo;

            placementInfo.location.y = 0.0f;

            bool foundSomethingToPlace = false;
            do {
                do {
                    placementInfo.location.x = d(gen);
                    placementInfo.location.z = d(gen);
                } while (!onSurface(placementInfo.location.x, placementInfo.location.z));

                auto biomesAtLocation = terrainPtr->GetBiomesAt(placementInfo.location.x, placementInfo.location.z);
                for (auto& biomeAtLocation : biomesAtLocation) {
                    if (!biomeAtLocation->getObjects().empty()) {
                        foundSomethingToPlace = true;
                        break;
                    }
                }
            } while (!foundSomethingToPlace);

            placementInfo.location.y = terrainPtr->GetHeight(placementInfo.location.x, placementInfo.location.z);

            std::vector<StaticObjectPtr> objectsAtLocation;
            auto biomesAtLocation = terrainPtr->GetBiomesAt(placementInfo.location.x, placementInfo.location.z);
            for (auto& biomeAtLocation : biomesAtLocation) {
                auto& objects = biomeAtLocation->getObjects();
                objectsAtLocation.insert(objectsAtLocation.begin(), objects.begin(), objects.end());
            }

            std::uniform_int_distribution<size_t> d_i(0, objectsAtLocation.size() - 1);
            size_t objectRandomIndex = d_i(gen);
            placementInfo.objectPtr = objectsAtLocation[objectRandomIndex];

            placementInfos.push_back(placementInfo);
        }
    }

    const PlacementInfoVec& StaticObjectPlacer::getPlacementInfos() const
    {
        return placementInfos;
    }

    terrain::TerrainPtr &StaticObjectPlacer::getTerrain() {
        return terrainPtr;
    }

    const terrain::TerrainPtr &StaticObjectPlacer::getTerrain() const {
        return terrainPtr;
    }
}