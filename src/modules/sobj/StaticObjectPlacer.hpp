#ifndef __STATIC_OBJECT_PLACER_HPP_
#define __STATIC_OBJECT_PLACER_HPP_

#include <engine/foundation/objloader/OBJLoader.hpp>

#include <modules/terrain/Terrain.hpp>

#include <unordered_map>
#include <vector>
#include <random>

namespace mod::sobj
{
    struct PlacementInfo
    {
        glm::vec3 location;
        size_t objectIndex;
    };
    typedef std::vector<PlacementInfo>		PlacementInfoVec;
    typedef std::vector<PlacementInfoVec>	PlacementInfoMat;

    class StaticObjectPlacer
    {
    public:
        StaticObjectPlacer(const terrain::TerrainPtr& terrainPtr, size_t objectCount, float marginOffset);
        ~StaticObjectPlacer();

        void place();

        [[nodiscard]] const PlacementInfoMat& getPlacementInfosForBiomes() const;

        terrain::TerrainPtr& getTerrain();
        [[nodiscard]] const terrain::TerrainPtr& getTerrain() const;
    private:
        const size_t objectCount;
        const float marginOffset;

        mod::terrain::TerrainPtr terrainPtr;

        PlacementInfoMat placementInfosForBiomes;
    };
    typedef std::shared_ptr<StaticObjectPlacer> StaticObjectPlacerPtr;
}

#endif // !__STATIC_OBJECT_PLACER_HPP_

