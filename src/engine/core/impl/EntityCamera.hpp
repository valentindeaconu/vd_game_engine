#ifndef __ENTITY_CAMERA_HPP_
#define __ENTITY_CAMERA_HPP_

#include "../Camera.hpp"

// Forward declaration for terrain
namespace mod
{
	namespace terrain
	{
		class Terrain;
		typedef std::shared_ptr<Terrain> TerrainPtr;
	}
}

namespace vd 
{
	// Forward declaration for entity
	namespace object
	{
		class Entity;
		typedef std::shared_ptr<Entity>	EntityPtr;
	}

	namespace core
	{
		struct EntityCameraInitParameters
		{
			object::EntityPtr entityPtr;
			mod::terrain::TerrainPtr terrainPtr;
			glm::vec3 playerPositionOffset;
		};

		namespace impl
		{
			class EntityCamera : public Camera
			{
			public:
				EntityCamera(const InputHandlerPtr& inputHandlerPtr);
				~EntityCamera();

				void init(CameraInitParametersPtr parameters);
				void update();

			private:
				void computeZoom();
				void computePitch();
				void computeAngleAroundPlayer();

				float computeHorizontalDistance();
				float computeVerticalDistance();

				glm::vec3 computeCameraPosition(const glm::vec3& playerPosition,
					float playerAngle,
					float horizontalDistance,
					float verticalDistance);

				float pitch;
				float distanceFromPlayer;
				float angleAroundPlayer;

				object::EntityPtr entityPtr;
				mod::terrain::TerrainPtr terrainPtr;
				glm::vec3 offset;
			};
		}
	}   
}

#endif // !__ENTITY_CAMERA_HPP_