#include "Camera.hpp"

namespace vd
{
	namespace core
	{
		Camera::Camera(const InputHandlerPtr& inputHandlerPtr)
			: inputHandlerPtr(inputHandlerPtr)
		{
		}

		Camera::~Camera()
		{
		}

		void Camera::update()
		{
			prevPosition = position;
			prevForward = forward;
		}

		glm::mat4 Camera::getViewMatrix()
		{
			return glm::lookAt(position, position + forward, kUpVector);
		}

		bool Camera::isCameraMoved() const
		{
			return prevPosition != position;
		}

		bool Camera::isCameraRotated() const
		{
			return prevForward != forward;
		}

		glm::vec3& Camera::getPosition()
		{
			return position;
		}

		const glm::vec3& Camera::getPosition() const
		{
			return position;
		}
		
		void Camera::setPosition(const glm::vec3& position)
		{
			this->position = position;
		}

		glm::vec3& Camera::getTarget()
		{
			return target;
		}

		const glm::vec3& Camera::getTarget() const
		{
			return target;
		}

		void Camera::setTarget(const glm::vec3& target)
		{
			this->target = target;
		}
		
		void Camera::updatePositionVectors()
		{
			right = glm::normalize(glm::cross(forward, kUpVector));
			up = glm::normalize(glm::cross(right, forward));
		}
	}
}