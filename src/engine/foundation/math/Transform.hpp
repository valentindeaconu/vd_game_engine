#ifndef __TRANSFORM_HPP_
#define __TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vd
{
	namespace math
	{
		class Transform
		{
		public:
			Transform();

			glm::mat4 get() const;

			glm::mat4 getTranslation() const;
			glm::mat4 getRotation() const;
			glm::mat4 getScaling() const;

			void setTranslation(float x, float y, float z);
			void setTranslation(const glm::vec3& translation);
			const glm::vec3& getTranslationVector() const;

			void setScaling(float x, float y, float z);
			void setScaling(const glm::vec3& scaling);
			const glm::vec3& getScalingVector() const;

			void setXRotationAngle(float angle);
			float getXAxisRotationAngle() const;
			void setYRotationAngle(float angle);
			float getYAxisRotationAngle() const;
			void setZRotationAngle(float angle);
			float getZAxisRotationAngle() const;

			const glm::quat& getXAxisRotation() const;
			const glm::quat& getYAxisRotation() const;
			const glm::quat& getZAxisRotation() const;

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
}

#endif // !__TRANSFORM_HPP_
