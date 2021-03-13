//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_FREECAMERA_HPP
#define VD_GAME_ENGINE_FREECAMERA_HPP

#include <engine/camera/Camera.hpp>

#include <engine/logger/Logger.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

namespace vd::camera::impl {
    class FreeCamera : public camera::Camera {
    public:
        explicit FreeCamera(const std::string& propsFilePath);

        void Reflect(const Axis& axis, float amount) override;

        float& Speed();
        float& RotationSensitivity();

    private:
        friend class camera::CameraManager;

        void Move(const glm::vec3& direction, float amount);
        void Rotate(float pitch, float yaw);

        void UpdateForwardVector();

        float m_Speed;
        float m_RotationSensitivity;

        float m_Pitch;
        float m_Yaw;
    };
}

#endif //VD_GAME_ENGINE_FREECAMERA_HPP