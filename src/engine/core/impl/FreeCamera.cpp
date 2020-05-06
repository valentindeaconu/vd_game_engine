#include "FreeCamera.hpp"

namespace vd
{
	namespace core
	{
		namespace impl
		{
			FreeCamera::FreeCamera(const InputHandlerPtr& inputHandlerPtr)
				: Camera(inputHandlerPtr)
			{
			}

			FreeCamera::~FreeCamera()
			{
			}

			void FreeCamera::init(CameraInitParametersPtr parameters)
			{
				FreeCameraInitParameters* params = (FreeCameraInitParameters*)(parameters);

				position = params->initPosition;
				target = params->initTarget;
				speed = params->speed;

				forward = glm::normalize(target - position);
				updatePositionVectors();

				const glm::vec3 x_unit(1.0f, 0.0f, 0.0f);
				const glm::vec3 y_unit(0.0f, 1.0f, 0.0f);
				const glm::vec3 z_unit(0.0f, 0.0f, 1.0f);

				glm::vec3 fwrd = forward;

				pitch = -glm::degrees(-glm::asin(glm::dot(fwrd, y_unit)));
				fwrd.y = 0.0f;
				fwrd = glm::normalize(fwrd);

				yaw = -glm::degrees(glm::acos(glm::dot(fwrd, x_unit)));

				if (glm::dot(fwrd, z_unit) > 0)
				{
					yaw = 360 - yaw;
				}
			}

			void FreeCamera::update()
			{
				float movAmt = speed * 0.5f;
				float rotAmt = speed * 2.0f;

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_W))
					move(forward, movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_S))
					move(forward, -movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_A))
					move(right, -movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_D))
					move(right, movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_T))
					move(up, movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_G))
					move(up, -movAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_LEFT))
					rotate(0.0f, -rotAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_RIGHT))
					rotate(0.0f, rotAmt);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_UP))
					rotate(rotAmt, 0.0f);

				if (inputHandlerPtr->getKeyHolding(GLFW_KEY_DOWN))
					rotate(-rotAmt, 0.0f);

				Camera::update();
			}

			void FreeCamera::move(const glm::vec3& dir, float amount)
			{
				position += dir * amount;
			}

			void FreeCamera::rotate(float pitch, float yaw)
			{
				this->pitch = std::clamp(this->pitch + pitch, -89.0f, 89.0f);
				this->yaw += yaw;

				forward.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
				forward.y = std::sin(glm::radians(this->pitch));
				forward.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
				forward = glm::normalize(forward);

				updatePositionVectors();
			}
		}
	}
}
