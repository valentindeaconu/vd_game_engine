#include "Window.hpp"

namespace vd::core
{
    Window::Window(const InputHandlerPtr& inputHandlerPtr)
        : inputHandlerPtr(inputHandlerPtr)
        , window(nullptr)
        , width(1280)
        , height(720)
        , hasChanged(false)
    {
    }

    Window::~Window() = default;

    void Window::create(uint32_t width, uint32_t height, const char* title) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (nullptr == window) {
            glfwTerminate();
            vd::Logger::terminate("Failed to create GLFW window", 1);
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, Window::windowResizeCallback);
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit())
        {
            vd::Logger::terminate("Failed to initialize GLEW", 1);
        }

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

        glViewport(0, 0, screenWidth, screenHeight);
        this->width = width;
        this->height = height;

        glfwSetKeyCallback(window, Window::keyboardCallback);
        glfwSetCursorPosCallback(window, Window::mouseCallback);
        glfwSetMouseButtonCallback(window, Window::mouseClickCallback);
        glfwSetScrollCallback(window, Window::mouseScrollCallback);

        // get version info
        const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
        auto version = glGetString(GL_VERSION); // version as a string

        std::stringstream rendererStream;
        rendererStream << "Renderer: " << renderer;
        vd::Logger::log(rendererStream.str());

        std::stringstream openGlVersionStream;
        openGlVersionStream << "OpenGL version supported " << version;
        vd::Logger::log(openGlVersionStream.str());

        hasChanged = true;
    }

    void Window::update() {
        if (hasChanged) {
            hasChanged = false;
        }

        glfwSwapBuffers(window);

        if (inputHandlerPtr->getWindowResized())
        {
            WindowInfo info = inputHandlerPtr->getWindowSize();
            resize(info.width, info.height);
        }
    }

    void Window::dispose()
    {
        glfwDestroyWindow(window);
    }

    GLFWwindow* Window::getWindow() {
        return window;
    }

    bool Window::isCloseRequested()
    {
        return glfwWindowShouldClose(window);
    }

    bool Window::isPerspectiveChanged() const {
        return hasChanged;
    }

    void Window::resize(int width, int height)
    {
        if (width < 800 || height < 600)
        {
            vd::Logger::warn("Window cannot be resized below 800 x 600");
            return;
        }

        this->width = width;
        this->height = height;

        glfwSetWindowSize(window, width, height);

        glViewport(0, 0, width, height);

        vd::Logger::log("Window resized to " + std::to_string(width) + " x " + std::to_string(height));

        hasChanged = true;
    }

    int Window::getWidth() const
    {
        return width;
    }

    int Window::getHeight() const
    {
        return height;
    }

    float Window::getAspectRatio() const {
        return (float) width / (float) height;
    }

    glm::mat4 Window::getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), this->getAspectRatio(), Near_Plane, Far_Plane);
    }

    float Window::getFieldOfView() const
    {
        return fov;
    }

    void Window::setFieldOfView(float fov)
    {
        hasChanged = true;
        this->fov = fov;
    }

    float Window::getNearPlane() const
    {
        return Near_Plane;
    }

    float Window::getFarPlane() const
    {
        return Far_Plane;
    }

    void Window::windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height)
    {
        Window* wd = (Window*)glfwGetWindowUserPointer(window);
        wd->inputHandlerPtr->windowResizeCallback(window, width, height);
    }

    void Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        Window* wd = (Window*)glfwGetWindowUserPointer(window);
        wd->inputHandlerPtr->keyboardCallback(window, key, scancode, action, mode);
    }

    void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Window* wd = (Window*)glfwGetWindowUserPointer(window);
        wd->inputHandlerPtr->mouseCallback(window, xpos, ypos);
    }

    void Window::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Window* wd = (Window*)glfwGetWindowUserPointer(window);
        wd->inputHandlerPtr->mouseClickCallback(window, button, action, mods);
    }

    void Window::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Window* wd = (Window*)glfwGetWindowUserPointer(window);
        wd->inputHandlerPtr->mouseScrollCallback(window, xoffset, yoffset);
    }
}
