#include "Transform.hpp"

namespace vd
{
	namespace math
	{
		Transform::Transform()
			: translation(0.0f)
			, scaling(1.0f)
		{
			setXRotationAngle(0.0f);
			setYRotationAngle(0.0f);
			setZRotationAngle(0.0f);
		}

		glm::mat4 Transform::get() const
		{
			return getTranslation() * getScaling() * getRotation();
		}

		glm::mat4 Transform::getTranslation() const
		{
			return glm::translate(glm::mat4(1.0f), translation);
		}

		glm::mat4 Transform::getRotation() const
		{
			glm::mat4 rotX = glm::mat4_cast(xAxisRotation);
			glm::mat4 rotY = glm::mat4_cast(yAxisRotation);
			glm::mat4 rotZ = glm::mat4_cast(zAxisRotation);

			return rotZ * rotY * rotX;
		}

		glm::mat4 Transform::getScaling() const
		{
			return glm::scale(glm::mat4(1.0f), scaling);
		}

		void Transform::setTranslation(float x, float y, float z)
		{
			this->translation.x = x;
			this->translation.y = y;
			this->translation.z = z;
		}

		void Transform::setTranslation(const glm::vec3& translation)
		{
			setTranslation(translation.x, translation.y, translation.z);
		}

		const glm::vec3& Transform::getTranslationVector() const
		{
			return translation;
		}

		void Transform::setScaling(float x, float y, float z)
		{
			this->scaling.x = x;
			this->scaling.y = y;
			this->scaling.z = z;
		}

		void Transform::setScaling(const glm::vec3& scaling)
		{
			setScaling(scaling.x, scaling.y, scaling.z);
		}

		const glm::vec3& Transform::getScalingVector() const
		{
			return scaling;
		}

		void Transform::setXRotationAngle(float angle)
		{
			angle = glm::clamp(angle, 0.0f, 360.0f);
			xAxisRotationAngle = angle;
			xAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		float Transform::getXAxisRotationAngle() const
		{
			return xAxisRotationAngle;
		}

		const glm::quat& Transform::getXAxisRotation() const
		{
			return xAxisRotation;
		}

		float Transform::getYAxisRotationAngle() const
		{
			return yAxisRotationAngle;
		}

		void Transform::setYRotationAngle(float angle)
		{
			angle = glm::clamp(angle, 0.0f, 360.0f);
			yAxisRotationAngle = angle;
			yAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		const glm::quat& Transform::getYAxisRotation() const
		{
			return yAxisRotation;
		}

		void Transform::setZRotationAngle(float angle)
		{
			angle = glm::clamp(angle, 0.0f, 360.0f);
			zAxisRotationAngle = angle;
			zAxisRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		float Transform::getZAxisRotationAngle() const
		{
			return zAxisRotationAngle;
		}

		const glm::quat& Transform::getZAxisRotation() const
		{
			return zAxisRotation;
		}
	}
}