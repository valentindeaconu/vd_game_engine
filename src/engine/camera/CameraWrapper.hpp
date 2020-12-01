//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_CAMERAWRAPPER_HPP
#define VD_GAME_ENGINE_CAMERAWRAPPER_HPP

#include "Camera.hpp"

namespace vd::camera::wrapper {
    class CameraWrapper : public Camera {
    public:
        CameraPtr& Camera();

        void Reflect(const Axis& axis, float amount) override;

        glm::mat4 ViewMatrix() override;

        [[nodiscard]] bool CameraMoved() const override;
        [[nodiscard]] bool CameraRotated() const override;

        glm::vec3& Position() override;

        glm::vec3& Target() override;

        [[nodiscard]] const glm::vec3& Forward() const override;
        [[nodiscard]] const glm::vec3& Right() const override;
        [[nodiscard]] const glm::vec3& Up() const override;

        [[nodiscard]] float Pitch() const override;
        [[nodiscard]] float Yaw() const override;
        [[nodiscard]] float Roll() const override;
    private:
        CameraPtr m_pImpl;
    };
}


#endif //VD_GAME_ENGINE_CAMERAWRAPPER_HPP
