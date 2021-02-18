//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_WINDOW_HPP
#define VD_GAME_ENGINE_WINDOW_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

#include <engine/api/gl/GL.hpp>

#include <engine/misc/Types.hpp>

#include <engine/logger/Logger.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/event/EventHandler.hpp>

#include <engine/component/IManager.hpp>

namespace vd::window {
    class WindowManager;

    class Window {
    public:
        Window(uint32_t width, uint32_t height, const std::string& title);
        ~Window();

        void Dispose();
        void Resize(uint32_t width, uint32_t height);

        [[nodiscard]] uint32_t Width() const;
        [[nodiscard]] uint32_t Height() const;

        float& NearPlane();
        float& FarPlane();
        float& FieldOfView();

        [[nodiscard]] float AspectRatio() const;

        [[nodiscard]] bool PerspectiveChanged() const;
        [[nodiscard]] bool CloseRequested() const;

        [[nodiscard]] glm::mat4 ProjectionMatrix() const;
        [[nodiscard]] glm::mat4 OrthoProjectionMatrix() const;
    private:
        friend class WindowManager;

        static void WindowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
        static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
        static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
        static void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

        float m_NearPlane;
        float m_FarPlane;
        float m_FieldOfView;

        GLFWwindow* m_Window;

        vd::Dimension m_Dimension;

        bool m_Changed;
    };
    typedef std::shared_ptr<Window>	WindowPtr;

    class WindowManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        WindowManager();
        ~WindowManager();

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        WindowPtr m_WindowPtr;
        event::EventHandlerPtr m_pEventHandler;
    };
    typedef std::shared_ptr<WindowManager>  WindowManagerPtr;
}

#endif //VD_GAME_ENGINE_WINDOWMANAGER_HPP