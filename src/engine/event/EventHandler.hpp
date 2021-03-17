//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_EVENT_HPP
#define VD_GAME_ENGINE_EVENT_HPP

#include <engine/api/gl/GL.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <algorithm>
#include <memory>

#include <engine/misc/Types.hpp>
#include <engine/component/IManager.hpp>

#include <engine/injector/ObjectOfType.hpp>

namespace vd::event {
    class EventHandlerManager;

    class EventHandler {
    public:
        EventHandler();

        [[nodiscard]] bool KeyDown(int key) const;
        [[nodiscard]] bool KeyReleased(int key) const;
        [[nodiscard]] bool KeyHolding(int key) const;

        [[nodiscard]] bool ButtonReleased(int button) const;
        [[nodiscard]] bool ButtonDown(int button) const;
        [[nodiscard]] bool ButtonHolding(int button) const;

        float& MouseSensitivity();

        [[nodiscard]] bool MouseMoved() const;
        [[nodiscard]] bool MouseScrolled() const;

        [[nodiscard]] double MouseDX() const;
        [[nodiscard]] double MouseDY() const;
        [[nodiscard]] double MouseDWheel() const;

        [[nodiscard]] glm::vec2 MousePosition() const;

        [[nodiscard]] bool WindowResized() const;
        [[nodiscard]] vd::Dimension WindowSize() const;

        void WindowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
        void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
        void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
        void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

    private:
        const int kKeyCount = 1024;
        const int kButtonCount = 8;

        friend class EventHandlerManager;

        float m_MouseSensitivity;

        enum KeyStatus {
            eKeyFree = 0,
            eKeyPressed,
            eKeyPostPressed,
            eKeyHolding,
            eKeyReleased,
            eKeyPostReleased
        };

        enum ButtonStatus {
            eButtonFree = 0,
            eButtonPressed,
            eButtonPostPressed,
            eButtonHolding,
            eButtonReleased,
            eButtonPostReleased
        };

        enum MouseWheelStatus {
            eWheelFree = 0,
            eWheelPreUpdated,
            eWheelPostUpdated
        };

        enum MouseMoveStatus {
            eMoveFree = 0,
            eMovePreUpdated,
            eMovePostUpdated
        };

        enum WindowStatus {
            eWindowFree = 0,
            eWindowPreResize,
            eWindowPostResize
        };

        struct {
            glm::vec2 curr;
            glm::vec2 offset;
            MouseMoveStatus status;
        } m_MouseMovement;

        struct {
            glm::vec2 offset;
            MouseWheelStatus status;
        } m_MouseScroll;

        struct {
            vd::Dimension info;
            WindowStatus status;
        } m_WindowInfo;

        std::vector<KeyStatus> m_KeysStatus;
        std::vector<ButtonStatus> m_ButtonsStatus;
    };
    typedef std::shared_ptr<EventHandler>	EventHandlerPtr;

    class EventHandlerManager : public vd::component::IManager {
    public:
        EventHandlerManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        EventHandlerPtr m_EventHandlerPtr;
    };
    typedef std::shared_ptr<EventHandlerManager>    EventHandlerManagerPtr;

}

#endif //VD_GAME_ENGINE_EVENT_HPP