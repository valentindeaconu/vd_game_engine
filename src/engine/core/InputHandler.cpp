#include "InputHandler.hpp"

namespace vd
{
	namespace core
	{
		InputHandler::InputHandler()
		{
			this->keysStatus.resize(1024);
			this->buttonsStatus.resize(8);
		}

		InputHandler::~InputHandler()
		{
		}

		void InputHandler::update()
		{
			glfwPollEvents();

			for (size_t i = 0; i < 1024; ++i)
			{
				switch (keysStatus[i])
				{
				case eKeyPressed:
					keysStatus[i] = eKeyPostPressed;
					break;
				case eKeyPostPressed:
					keysStatus[i] = eKeyHolding;
					break;
				case eKeyReleased:
					keysStatus[i] = eKeyPostReleased;
					break;
				case eKeyPostReleased:
					keysStatus[i] = eKeyFree;
					break;
				default:
					break;
				}
			}

			for (size_t i = 0; i < 8; ++i)
			{
				switch (buttonsStatus[i])
				{
				case eButtonPressed:
					buttonsStatus[i] = eButtonPostPressed;
					break;
				case eButtonPostPressed:
					buttonsStatus[i] = eButtonHolding;
					break;
				case eButtonReleased:
					buttonsStatus[i] = eButtonPostReleased;
					break;
				case eButtonPostReleased:
					buttonsStatus[i] = eButtonFree;
					break;
				default:
					break;
				}
			}

			if (mouseMovement.status == eMovePreUpdated)
			{
				mouseMovement.status = eMovePostUpdated;
			}
			else if (mouseMovement.status == eMovePostUpdated)
			{
				mouseMovement.status = eMoveFree;
			}

			if (mouseScroll.status == eWheelPreUpdated)
			{
				mouseScroll.status = eWheelPostUpdated;
			}
			else if (mouseScroll.status == eWheelPostUpdated)
			{
				mouseScroll.status = eWheelFree;
			}

			if (windowInfo.status == eWindowPreResize)
			{
				windowInfo.status = eWindowPostResize;
			}
			else if (windowInfo.status == eWindowPostResize)
			{
				windowInfo.status = eWindowFree;
			}
		}


		void InputHandler::clear()
		{
			keysStatus.clear();
			keysStatus.resize(1024);
			buttonsStatus.clear();
			buttonsStatus.resize(8);
		}

		bool InputHandler::getKeyDown(int key)
		{
			return keysStatus[key] == eKeyPressed || keysStatus[key] == eKeyPostPressed;
		}

		bool InputHandler::getKeyReleased(int key)
		{
			return keysStatus[key] == eKeyReleased || keysStatus[key] == eKeyPostReleased;
		}

		bool InputHandler::getKeyHolding(int key)
		{
			return keysStatus[key] == eKeyHolding;
		}

		bool InputHandler::getButtonDown(int button)
		{
			return buttonsStatus[button] == eButtonPressed || buttonsStatus[button] == eButtonPostPressed;
		}

		bool InputHandler::getButtonReleased(int button)
		{
			return buttonsStatus[button] == eButtonReleased || buttonsStatus[button] == eButtonPostReleased;
		}

		bool InputHandler::getButtonHolding(int button)
		{
			return buttonsStatus[button] == eButtonHolding;
		}

		bool InputHandler::getMouseMoved()
		{
			return mouseMovement.status == eMovePreUpdated || mouseMovement.status == eMovePostUpdated;
		}

		bool InputHandler::getMouseScrolled()
		{
			return mouseScroll.status == eWheelPreUpdated || mouseScroll.status == eWheelPostUpdated;
		}

		double InputHandler::getMouseDX()
		{
			return mouseMovement.offset.x;
		}

		double InputHandler::getMouseDY()
		{
			return mouseMovement.offset.y;
		}

		double InputHandler::getMouseDWheel()
		{
			return mouseScroll.offset.y;
		}

		glm::vec2 InputHandler::getMousePosition()
		{
			return glm::vec2(mouseMovement.curr.x, mouseMovement.curr.y);;
		}

		bool InputHandler::getWindowResized()
		{
			return windowInfo.status == eWindowPreResize || windowInfo.status == eWindowPostResize;
		}

		WindowInfo InputHandler::getWindowSize()
		{
			return windowInfo.info;
		}

		void InputHandler::windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height)
		{
			glfwGetFramebufferSize(window, &this->windowInfo.info.width, &this->windowInfo.info.height);

			this->windowInfo.status = eWindowPreResize;
		}

		void InputHandler::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);

			if (key >= 0 && key < 1024)
			{
				if (action == GLFW_PRESS)
				{
					if (this->keysStatus[key] != eKeyHolding)
						this->keysStatus[key] = eKeyPressed;
				}
				else if (action == GLFW_RELEASE)
				{
					this->keysStatus[key] = eKeyReleased;
				}
			}
		}

		void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
		{
			this->mouseMovement.status = eMovePreUpdated;
			this->mouseMovement.offset.x = (xpos - this->mouseMovement.curr.x) * this->kMouseSensivity;
			this->mouseMovement.offset.y = (this->mouseMovement.curr.y - ypos) * this->kMouseSensivity;

			this->mouseMovement.curr.x = xpos;
			this->mouseMovement.curr.y = ypos;
		}

		void InputHandler::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (button >= 0 && button < 8)
			{
				if (action == GLFW_PRESS)
				{
					if (this->buttonsStatus[button] != eButtonHolding)
						this->buttonsStatus[button] = eButtonPressed;
				}
				else if (action == GLFW_RELEASE)
				{
					this->buttonsStatus[button] = eButtonReleased;
				}
			}
		}

		void InputHandler::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			this->mouseScroll.status = eWheelPreUpdated;
			this->mouseScroll.offset.x = xoffset;
			this->mouseScroll.offset.y = yoffset;
		}
	}
}

