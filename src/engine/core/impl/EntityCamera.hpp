#ifndef __ENTITY_CAMERA_HPP_
#define __ENTITY_CAMERA_HPP_

#include "../Camera.hpp"

// Forward declaration for terrain
namespace mod::terrain
{
    class Terrain;
    typedef std::shared_ptr<Terrain> TerrainPtr;
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
				explicit EntityCamera(const InputHandlerPtr& inputHandlerPtr);
				~EntityCamera();

				void init(CameraInitParametersPtr parameters) override;
				void update() override;

				void reflect(float yAxisSymmetric) override;

			private:
				void input();

				[[nodiscard]] float computeHorizontalDistance() const;
				[[nodiscard]] float computeVerticalDistance() const;

				[[nodiscard]] glm::vec3 computeCameraPosition(const glm::vec3& playerPosition,
					float playerAngle,
					float horizontalDistance,
					float verticalDistance) const;

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