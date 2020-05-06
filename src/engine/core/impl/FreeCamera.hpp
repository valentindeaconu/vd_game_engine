#ifndef __FREE_CAMERA_HPP_
#define __FREE_CAMERA_HPP_

#include "../Camera.hpp"

namespace vd 
{
	namespace core
	{
		struct FreeCameraInitParameters
		{
			float speed;
			glm::vec3 initPosition;
			glm::vec3 initTarget;
		};

		namespace impl
		{
			class FreeCamera : public Camera
			{
			public:
				FreeCamera(const InputHandlerPtr& inputHandlerPtr);
				~FreeCamera();

				void init(CameraInitParametersPtr parameters);
				void update();

			private:
				void move(const glm::vec3& dir, float amount);
				void rotate(float pitch, float yaw);

				float speed;

				float pitch;
				float yaw;
			};
		}		
	}   
}

#endif // !__CAMERA_HPP_