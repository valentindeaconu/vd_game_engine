#ifndef __STATIC_OBJECT_PLACER_HPP_
#define __STATIC_OBJECT_PLACER_HPP_

#include <engine/foundation/objloader/OBJLoader.hpp>

#include <modules/sky/Sky.hpp>

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
        StaticObjectPlacer(const mod::sky::SkyPtr& skyPtr, size_t objectCount, float marginOffset);
        ~StaticObjectPlacer();

        void place();

        const PlacementInfoMat& getPlacementInfosForBiomes() const;

        mod::sky::SkyPtr& getSky();
        const mod::sky::SkyPtr& getSky() const;
        void setSky(const mod::sky::SkyPtr& skyPtr);
    private:
        const size_t objectCount;
        const float marginOffset;

        mod::sky::SkyPtr skyPtr;

        PlacementInfoMat placementInfosForBiomes;
    };
    typedef std::shared_ptr<StaticObjectPlacer> StaticObjectPlacerPtr;
}

#endif // !__STATIC_OBJECT_PLACER_HPP_

