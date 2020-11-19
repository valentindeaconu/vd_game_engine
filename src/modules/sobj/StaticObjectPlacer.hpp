#ifndef __STATIC_OBJECT_PLACER_HPP_
#define __STATIC_OBJECT_PLACER_HPP_

#include <engine/injector/ObjectOfType.hpp>
#include <modules/terrain/Terrain.hpp>

#include <unordered_map>
#include <vector>
#include <random>

namespace mod::sobj {
    struct PlacementInfo {
        glm::vec3 location;
        StaticObjectPtr objectPtr;
    };
    typedef std::vector<PlacementInfo>		PlacementInfoVec;

    class StaticObjectPlacer {
    public:
        StaticObjectPlacer(size_t objectCount, float marginOffset);
        ~StaticObjectPlacer();

        void place();

        [[nodiscard]] const PlacementInfoVec& getPlacementInfos() const;

        terrain::TerrainPtr& getTerrain();
        [[nodiscard]] const terrain::TerrainPtr& getTerrain() const;
    private:
        const size_t objectCount;
        const float marginOffset;

        mod::terrain::TerrainPtr terrainPtr;

        PlacementInfoVec placementInfos;
    };
    typedef std::shared_ptr<StaticObjectPlacer> StaticObjectPlacerPtr;
}

#endif // !__STATIC_OBJECT_PLACER_HPP_

