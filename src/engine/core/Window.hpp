#ifndef __WINDOW_HPP_
#define __WINDOW_HPP_

#include <cstdint>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/foundation/GL.hpp>

#include "InputHandler.hpp"

namespace vd
{
	namespace core
	{
		class Window {

		public:
			Window(const InputHandlerPtr& inputHandlerPtr);
			~Window();

			void create(uint32_t width, uint32_t height, const char* title);
			void update();
			void dispose();
			GLFWwindow* getWindow();

			int getWidth() const;
			int getHeight() const;

			void resize(int width, int height);

			float getFieldOfView() const;
			void setFieldOfView(float fov);

			float getNearPlane() const;
			float getFarPlane() const;

			bool isCloseRequested();

			glm::mat4 getProjectionMatrix() const;
		private:
			static void windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
			static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
			static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
			static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			float Near_Plane = 0.1f;
			float Far_Plane = 10000.0f;
			float fov = 45.0f;

			GLFWwindow* window;
			int width;
			int height;

			InputHandlerPtr inputHandlerPtr;
		};
		typedef std::shared_ptr<Window>	WindowPtr;
	}
}

#endif // !__WINDOW_HPP_