//
// Created by Vali on 11/12/2020.
//

#include "Window.hpp"

namespace vd::window {
    Window::Window(uint32_t width, uint32_t height, const std::string& title)
        : m_Window(nullptr)
        , m_Dimension(width, height)
        , m_Changed(false)
        , m_NearPlane(0.1f)
        , m_FarPlane(10000.0f)
        , m_FieldOfView(45.0f)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (nullptr == m_Window) {
            glfwTerminate();
            throw std::runtime_error("failed to create GLFW window");
        }

        glfwSetWindowUserPointer(m_Window, this);
        glfwSetWindowSizeCallback(m_Window, Window::WindowResizeCallback);
        glfwMakeContextCurrent(m_Window);

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            throw std::runtime_error("failed to initialize GLEW");
        }

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_Window, &screenWidth, &screenHeight);

        glViewport(0, 0, screenWidth, screenHeight);
        this->m_Dimension = vd::Dimension(width, height);

        glfwSetKeyCallback(m_Window, Window::KeyboardCallback);
        glfwSetCursorPosCallback(m_Window, Window::MouseCallback);
        glfwSetMouseButtonCallback(m_Window, Window::MouseClickCallback);
        glfwSetScrollCallback(m_Window, Window::MouseScrollCallback);

        // get version info
        const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
        auto version = glGetString(GL_VERSION); // version as a string

        std::stringstream rendererStream;
        rendererStream << "Renderer: " << renderer;
        vd::Logger::log(rendererStream.str());

        std::stringstream openGlVersionStream;
        openGlVersionStream << "OpenGL version supported " << version;
        vd::Logger::log(openGlVersionStream.str());

        m_Changed = true;
    }

    Window::~Window() = default;

    void Window::Dispose() {
        glfwDestroyWindow(m_Window);
    }

    void Window::Resize(uint32_t width, uint32_t height) {
        if (width < 800 || height < 600) {
            vd::Logger::warn("Window cannot be resized below 800 x 600");

            width = 800;
            height = 600;
        }

        this->m_Dimension = vd::Dimension(width, height);

        glfwSetWindowSize(m_Window, width, height);

        glViewport(0, 0, width, height);

        vd::Logger::log("Window resized to " + std::to_string(width) + " x " + std::to_string(height));

        m_Changed = true;
    }

    uint32_t Window::Width() const {
        return m_Dimension.width;
    }

    uint32_t Window::Height() const {
        return m_Dimension.height;
    }

    float& Window::NearPlane() {
        return m_NearPlane;
    }

    float& Window::FarPlane() {
        return m_FarPlane;
    }

    float& Window::FieldOfView() {
        return m_FieldOfView;
    }

    float Window::AspectRatio() const {
        return (float) m_Dimension.width / (float) m_Dimension.height;
    }

    bool Window::PerspectiveChanged() const {
        return m_Changed;
    }

    bool Window::CloseRequested() const {
        return glfwWindowShouldClose(m_Window);
    }

    glm::mat4 Window::ProjectionMatrix() const {
        return glm::perspective(glm::radians(m_FieldOfView), this->AspectRatio(), m_NearPlane, m_FarPlane);
    }

    void Window::WindowResizeCallback(GLFWwindow* window, int32_t width, int32_t height) {
        auto& inputHandler = vd::ObjectOfType<event::EventHandler>::Find();
        inputHandler->WindowResizeCallback(window, width, height);
    }

    void Window::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        auto& inputHandler = vd::ObjectOfType<event::EventHandler>::Find();
        inputHandler->KeyboardCallback(window, key, scancode, action, mode);
    }

    void Window::MouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
        auto& inputHandler = vd::ObjectOfType<event::EventHandler>::Find();
        inputHandler->MouseCallback(window, x_pos, y_pos);
    }

    void Window::MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
        auto& inputHandler = vd::ObjectOfType<event::EventHandler>::Find();
        inputHandler->MouseClickCallback(window, button, action, mods);
    }

    void Window::MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
        auto& inputHandler = vd::ObjectOfType<event::EventHandler>::Find();
        inputHandler->MouseScrollCallback(window, x_offset, y_offset);
    }

    WindowManager::WindowManager() {
        try {
            m_WindowPtr = std::make_shared<Window>(1280, 720, "VDGE");
        } catch (std::runtime_error& e) {
            vd::Logger::terminate(e.what(), 1);
        }

        m_WindowPtr->NearPlane() = 0.1f;
        m_WindowPtr->FarPlane() = 10000.0f;
        m_WindowPtr->FieldOfView() = 45.0f;

        vd::ObjectOfType<Window>::Provide(m_WindowPtr);
    }

    WindowManager::~WindowManager() = default;

    void WindowManager::Init() {
        m_EventHandlerPtr = vd::ObjectOfType<event::EventHandler>::Find();
    }

    void WindowManager::Update() {
        if (m_WindowPtr->m_Changed) {
            m_WindowPtr->m_Changed = false;
        }

        glfwSwapBuffers(m_WindowPtr->m_Window);

        if (m_EventHandlerPtr->WindowResized()) {
            auto info = m_EventHandlerPtr->WindowSize();
            m_WindowPtr->Resize(info.width, info.height);
        }
    }

    void WindowManager::CleanUp() {
        m_WindowPtr->Dispose();
    }
}
