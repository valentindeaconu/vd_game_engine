#ifndef __INPUT_HANDLER_HPP_
#define __INPUT_HANDLER_HPP_

#include <engine/foundation/GL.hpp>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

namespace vd
{
	namespace core
	{
		struct WindowInfo
		{
			int width;
			int height;
		};

		class InputHandler
		{
		public:
			InputHandler();
			~InputHandler();

			void update();
			void clear();

			bool getKeyDown(int key);
			bool getKeyReleased(int key);
			bool getKeyHolding(int key);
			bool getButtonReleased(int button);
			bool getButtonDown(int button);
			bool getButtonHolding(int button);

			bool getMouseMoved();
			bool getMouseScrolled();

			double getMouseDX();
			double getMouseDY();
			double getMouseDWheel();

			glm::vec2 getMousePosition();

			bool getWindowResized();
			WindowInfo getWindowSize();

			void windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
			void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
			void mouseCallback(GLFWwindow* window, double xpos, double ypos);
			void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
			void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		private:
			enum KeyStatus
			{
				eKeyFree = 0,
				eKeyPressed,
				eKeyPostPressed,
				eKeyHolding,
				eKeyReleased,
				eKeyPostReleased
			};

			enum ButtonStatus
			{
				eButtonFree = 0,
				eButtonPressed,
				eButtonPostPressed,
				eButtonHolding,
				eButtonReleased,
				eButtonPostReleased
			};

			enum MouseWheelStatus
			{
				eWheelFree = 0,
				eWheelPreUpdated,
				eWheelPostUpdated
			};

			struct
			{
				struct
				{
					double x;
					double y;
				} offset;

				MouseWheelStatus status;
			} mouseScroll;

			enum MouseMoveStatus
			{
				eMoveFree = 0,
				eMovePreUpdated,
				eMovePostUpdated
			};

			struct
			{
				struct
				{
					double x;
					double y;
				} curr;

				struct
				{
					double x;
					double y;
				} offset;

				MouseMoveStatus status;
			} mouseMovement;

			const float kMouseSensivity = 0.03f;

			enum WindowStatus
			{
				eWindowFree = 0,
				eWindowPreResize,
				eWindowPostResize
			};

			struct
			{
				WindowInfo info;

				WindowStatus status;
			} windowInfo;

			std::vector<KeyStatus> keysStatus;
			std::vector<ButtonStatus> buttonsStatus;
		};
		typedef std::shared_ptr<InputHandler>	InputHandlerPtr;

	}
}

#endif // !__INPUT_HANDLER_HPP_