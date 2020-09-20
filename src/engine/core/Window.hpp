#ifndef __WINDOW_HPP_
#define __WINDOW_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/foundation/GL.hpp>
#include <engine/logger/Logger.hpp>

#include "InputHandler.hpp"

namespace vd::core
{
    class Window {

    public:
        explicit Window(const InputHandlerPtr& inputHandlerPtr);
        ~Window();

        void create(uint32_t width, uint32_t height, const char* title);
        void update();
        void dispose();
        GLFWwindow* getWindow();

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

        [[nodiscard]] float getAspectRatio() const;

        void resize(int width, int height);

        [[nodiscard]] float getFieldOfView() const;
        void setFieldOfView(float fov);

        [[nodiscard]] float getNearPlane() const;
        [[nodiscard]] float getFarPlane() const;

        bool isCloseRequested();
        bool isPerspectiveChanged() const;

        [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    private:
        static void windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
        static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        float Near_Plane = 0.1f;
        float Far_Plane = 1000.0f;
        float fov = 45.0f;

        GLFWwindow* window;
        int width;
        int height;

        bool hasChanged;

        InputHandlerPtr inputHandlerPtr;
    };
    typedef std::shared_ptr<Window>	WindowPtr;
}

#endif // !__WINDOW_HPP_