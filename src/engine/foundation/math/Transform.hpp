#ifndef __TRANSFORM_HPP_
#define __TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vd::math {
    class Transform {
    public:
        Transform();

        [[nodiscard]] glm::mat4 Get() const;
        [[nodiscard]] glm::mat4 Inverse() const;

        [[nodiscard]] glm::mat4 GetTranslation() const;
        [[nodiscard]] glm::mat4 GetRotation() const;
        [[nodiscard]] glm::mat4 GetScaling() const;

        void SetTranslation(float x, float y, float z);
        void SetTranslation(const glm::vec3& translation);
        [[nodiscard]] const glm::vec3& GetTranslationVector() const;

        void SetScaling(float x, float y, float z);
        void SetScaling(const glm::vec3& scaling);
        [[nodiscard]] const glm::vec3& GetScalingVector() const;

        void SetXRotationAngle(float angle);
        [[nodiscard]] float GetXAxisRotationAngle() const;

        void SetYRotationAngle(float angle);
        [[nodiscard]] float GetYAxisRotationAngle() const;

        void SetZRotationAngle(float angle);
        [[nodiscard]] float GetZAxisRotationAngle() const;

        [[nodiscard]] const glm::quat& GetXAxisRotation() const;
        [[nodiscard]] const glm::quat& GetYAxisRotation() const;
        [[nodiscard]] const glm::quat& GetZAxisRotation() const;

        [[nodiscard]] glm::vec4 operator*(const glm::vec4& operand) const;
    private:
        void ComputeTransform() const;

        bool m_TransformComputed;
        glm::mat4 m_Transform;

        glm::vec3 m_Translation;

        glm::vec3 m_Scaling;

        float m_XAxisRotationAngle;
        glm::quat m_XAxisRotation;

        float m_YAxisRotationAngle;
        glm::quat m_YAxisRotation;

        float m_ZAxisRotationAngle;
        glm::quat m_ZAxisRotation;
    };
}

#endif // !__TRANSFORM_HPP_
