#ifndef __TRANSFORM_HPP_
#define __TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vd::math
{
    class Transform
    {
    public:
        Transform();

        [[nodiscard]] glm::mat4 get() const;

        [[nodiscard]] glm::mat4 getTranslation() const;
        [[nodiscard]] glm::mat4 getRotation() const;
        [[nodiscard]] glm::mat4 getScaling() const;

        void setTranslation(float x, float y, float z);
        void setTranslation(const glm::vec3& translation);
        [[nodiscard]] const glm::vec3& getTranslationVector() const;

        void setScaling(float x, float y, float z);
        void setScaling(const glm::vec3& scaling);
        [[nodiscard]] const glm::vec3& getScalingVector() const;

        void setXRotationAngle(float angle);
        [[nodiscard]] float getXAxisRotationAngle() const;
        void setYRotationAngle(float angle);
        [[nodiscard]] float getYAxisRotationAngle() const;
        void setZRotationAngle(float angle);
        float getZAxisRotationAngle() const;

        [[nodiscard]] const glm::quat& getXAxisRotation() const;
        [[nodiscard]] const glm::quat& getYAxisRotation() const;
        [[nodiscard]] const glm::quat& getZAxisRotation() const;

        [[nodiscard]] glm::vec4 operator*(const glm::vec4& operand) const;
    private:
        glm::vec3 translation;

        glm::vec3 scaling;

        float xAxisRotationAngle;
        glm::quat xAxisRotation;

        float yAxisRotationAngle;
        glm::quat yAxisRotation;

        float zAxisRotationAngle;
        glm::quat zAxisRotation;
    };
}

#endif // !__TRANSFORM_HPP_
