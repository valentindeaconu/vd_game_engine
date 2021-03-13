#include "ThirdPersonCamera.hpp"

#include <modules/terrain/Terrain.hpp>
#include <modules/player/Player.hpp>

namespace vd::camera::impl {
    ThirdPersonCamera::ThirdPersonCamera()
        : Camera()
        , m_Pitch(0.0f)
        , m_DistanceFromPlayer(0.0f)
        , m_AngleAroundPlayer(0.0f)
        , m_Offset(0.0f)
    {
    }

    ThirdPersonCamera::ThirdPersonCamera(const ThirdPersonCamera& other)
        : Camera(other)
        , m_Pitch(other.m_Pitch)
        , m_DistanceFromPlayer(other.m_DistanceFromPlayer)
        , m_AngleAroundPlayer(other.m_AngleAroundPlayer)
        , m_PlayerTransformGetter(other.m_PlayerTransformGetter)
        , m_HeightGetter(other.m_HeightGetter)
        , m_Offset(other.m_Offset)
    {
    }

    void ThirdPersonCamera::Link() {
         auto pPlayer = vd::ObjectOfType<mod::player::Player>::Find();
         auto pTerrain = vd::ObjectOfType<mod::terrain::Terrain>::Find();

         m_PlayerTransformGetter = [p = pPlayer]() { return p->WorldTransform(); };

         m_HeightGetter = [t = pTerrain](float x, float z) { return t->HeightAt(x, z); };

         m_Offset = glm::vec3(0.0f, pPlayer->ModelYOffset(), 0.0f);
    }

    void impl::ThirdPersonCamera::Reflect(const Camera::Axis& axis, float amount) {
        switch (axis) {
            case eX: {
                float totalOffset = 2.0f * std::abs(m_Position.x - amount);

                if (m_Position.x > amount) {
                    m_Position.x -= totalOffset;
                } else {
                    m_Position.x += totalOffset;
                }

                m_Forward.x = -m_Forward.x;
                UpdatePositionVectors();
                break;
            }
            case eY: {
                float totalOffset = 2.0f * std::abs(m_Position.y - amount);

                if (m_Position.y > amount) {
                    m_Position.y -= totalOffset;
                } else {
                    m_Position.y += totalOffset;
                }

                m_Forward.y = -m_Forward.y;
                UpdatePositionVectors();
                break;
            }
            case eZ: {
                float totalOffset = 2.0f * std::abs(m_Position.z - amount);

                if (m_Position.z > amount) {
                    m_Position.z -= totalOffset;
                } else {
                    m_Position.z += totalOffset;
                }

                m_Forward.z = -m_Forward.z;
                UpdatePositionVectors();
                break;
            }
        }
    }

    float& ThirdPersonCamera::Pitch() {
        return m_Pitch;
    }

    float& ThirdPersonCamera::DistanceFromPlayer() {
        return m_DistanceFromPlayer;
    }

    float& ThirdPersonCamera::AngleAroundPlayer() {
        return m_AngleAroundPlayer;
    }

    void ThirdPersonCamera::Update() {
        float horizontalDistance = HorizontalDistance();
        float verticalDistance = VerticalDistance();

        auto worldTransform = m_PlayerTransformGetter();
        glm::vec3 entityPosition = worldTransform.Translation() + m_Offset;
        float entityAngle = worldTransform.YAxisRotationAngle();

        glm::vec3 newPosition = CameraPosition(entityPosition, entityAngle, horizontalDistance, verticalDistance);

        float allowedHeight = m_HeightGetter(newPosition.x, newPosition.z) + m_Offset.y;

        if (newPosition.y < allowedHeight) {
            newPosition.y = allowedHeight;
        }

        m_Position = newPosition;

        m_Forward = glm::normalize(entityPosition - m_Position);
        UpdatePositionVectors();
    }

    float ThirdPersonCamera::HorizontalDistance() const {
        return m_DistanceFromPlayer * glm::cos(glm::radians(m_Pitch));
    }

    float ThirdPersonCamera::VerticalDistance() const {
        return m_DistanceFromPlayer * glm::sin(glm::radians(m_Pitch));
    }

    glm::vec3 ThirdPersonCamera::CameraPosition(const glm::vec3& playerPosition,
                                                float playerAngle,
                                                float horizontalDistance,
                                                float verticalDistance) const {
        float theta = playerAngle + m_AngleAroundPlayer;
        float xOffset = horizontalDistance * glm::sin(glm::radians(theta));
        float zOffset = horizontalDistance * glm::cos(glm::radians(theta));
        glm::vec3 res;
        res.x = playerPosition.x - xOffset;
        res.z = playerPosition.z - zOffset;
        res.y = playerPosition.y + verticalDistance;

        return res;
    }
}
