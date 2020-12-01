//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_TRANSFORM_HPP
#define VD_GAME_ENGINE_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vd::math {
    class Transform {
    public:
        Transform();

        [[nodiscard]] glm::mat4 Get() const;
        [[nodiscard]] glm::mat4 Inverse() const;

        [[nodiscard]] glm::mat4 TranslationMatrix() const;
        [[nodiscard]] glm::mat4 RotationMatrix() const;
        [[nodiscard]] glm::mat4 ScaleMatrix() const;

        glm::vec3& Translation();
        glm::vec3& Scale();

        float& XAxisRotationAngle();
        float& YAxisRotationAngle();
        float& ZAxisRotationAngle();

        [[nodiscard]] glm::vec4 operator*(const glm::vec4& operand) const;
    private:
        void ComputeTransform() const;

        void ComputeXAxisRotationAngle();
        void ComputeYAxisRotationAngle();
        void ComputeZAxisRotationAngle();

        bool m_TransformComputed;
        glm::mat4 m_Transform;

        glm::vec3 m_Translation;
        glm::vec3 m_Scaling;

        glm::vec3 m_AxisRotationAngle;
        glm::u8vec3 m_AxisRotationAngleComputed;

        glm::quat m_XAxisRotation;
        glm::quat m_YAxisRotation;
        glm::quat m_ZAxisRotation;
    };
}

#endif //VD_GAME_ENGINE_TRANSFORM_HPP
