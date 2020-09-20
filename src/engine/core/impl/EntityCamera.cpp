#include "EntityCamera.hpp"

#include <engine/object/Entity.hpp>
#include <modules/terrain/Terrain.hpp>

namespace vd::core::impl
{
    EntityCamera::EntityCamera(const InputHandlerPtr& inputHandlerPtr)
        : Camera(inputHandlerPtr)
    {
    }

    EntityCamera::~EntityCamera() = default;

    void EntityCamera::init(CameraInitParametersPtr parameters)
    {
        EntityCameraInitParameters* params = (EntityCameraInitParameters*)parameters;

        entityPtr = params->entityPtr;
        terrainPtr = params->terrainPtr;
        offset = params->playerPositionOffset;

        distanceFromPlayer = 8.5f;
        pitch = 20.0f;
        angleAroundPlayer = 180.0f;
    }

    void EntityCamera::update()
    {
        computeZoom();
        computePitch();
        computeAngleAroundPlayer();

        float horizontalDistance = computeHorizontalDistance();
        float verticalDistance = computeVerticalDistance();

        auto worldTransform = entityPtr->getWorldTransform();
        glm::vec3 entityPosition = worldTransform.getTranslationVector() + offset;
        float entityAngle = worldTransform.getYAxisRotationAngle();

        glm::vec3 newPosition = computeCameraPosition(entityPosition,
                                                      entityAngle, horizontalDistance, verticalDistance);

        float allowedHeight =
                terrainPtr->getTerrainConfig()->getHeight(newPosition.x, newPosition.z) +
                offset.y;

        if (newPosition.y < allowedHeight)
        {
            newPosition.y = allowedHeight;
        }

        position = newPosition;

        forward = glm::normalize(entityPosition - position);
        updatePositionVectors();

        Camera::update();
    }

    void EntityCamera::invertPitch() {
        pitch = -pitch;

        float yaw = getYaw();

        forward.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        forward.y = std::sin(glm::radians(pitch));
        forward.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        forward = glm::normalize(forward);

        updatePositionVectors();
    }

    void EntityCamera::computeZoom()
    {
        if (inputHandlerPtr->getMouseScrolled())
        {
            float zoomLevel = (float)inputHandlerPtr->getMouseDWheel();
            distanceFromPlayer -= zoomLevel;
            distanceFromPlayer = glm::clamp(distanceFromPlayer, 2.5f, 35.0f);
        }
    }

    void EntityCamera::computePitch()
    {
        if (inputHandlerPtr->getButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE))
        {
            if (inputHandlerPtr->getMouseMoved())
            {
                float pitchChange = (float)inputHandlerPtr->getMouseDY();
                pitch -= pitchChange;
                pitch = glm::clamp(pitch, 5.0f, 75.0f);
            }
        }
    }

    void EntityCamera::computeAngleAroundPlayer()
    {
        if (inputHandlerPtr->getButtonHolding(GLFW_MOUSE_BUTTON_MIDDLE))
        {
            if (inputHandlerPtr->getMouseMoved())
            {
                float angleChange = (float)inputHandlerPtr->getMouseDX();
                angleAroundPlayer -= angleChange * 2.5f;
            }
        }
    }

    float EntityCamera::computeHorizontalDistance() const
    {
        return distanceFromPlayer * glm::cos(glm::radians(pitch));
    }

    float EntityCamera::computeVerticalDistance() const
    {
        return distanceFromPlayer * glm::sin(glm::radians(pitch));
    }

    glm::vec3 EntityCamera::computeCameraPosition(const glm::vec3& playerPosition, float playerAngle, float horizontalDistance, float verticalDistance) const
    {
        float theta = playerAngle + angleAroundPlayer;
        float xOffset = horizontalDistance * glm::sin(glm::radians(theta));
        float zOffset = horizontalDistance * glm::cos(glm::radians(theta));
        glm::vec3 res;
        res.x = playerPosition.x - xOffset;
        res.z = playerPosition.z - zOffset;
        res.y = playerPosition.y + verticalDistance;

        return res;
    }
}
