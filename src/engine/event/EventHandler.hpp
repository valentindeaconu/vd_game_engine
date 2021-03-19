//
// Created by Vali on 11/12/2020.
//

#ifndef VDGE_EVENT_HPP
#define VDGE_EVENT_HPP

#include <glm/glm.hpp>

#include <vector>
#include <algorithm>
#include <memory>

#include <engine/defines/Types.hpp>
#include <engine/component/IManager.hpp>

#include <engine/injector/ObjectOfType.hpp>

#include <engine/defines/Keys.hpp>
#include <engine/defines/Buttons.hpp>
#include <engine/defines/Actions.hpp>

namespace vd::event {
    class EventHandlerManager;

    class EventHandler {
    public:
        EventHandler();

        [[nodiscard]] bool KeyDown(Key::Code key) const;
        [[nodiscard]] bool KeyReleased(Key::Code key) const;
        [[nodiscard]] bool KeyHolding(Key::Code key) const;

        [[nodiscard]] bool ButtonReleased(Button::Code button) const;
        [[nodiscard]] bool ButtonDown(Button::Code button) const;
        [[nodiscard]] bool ButtonHolding(Button::Code button) const;

        float& MouseSensitivity();

        [[nodiscard]] bool MouseMoved() const;
        [[nodiscard]] bool MouseScrolled() const;

        [[nodiscard]] double MouseDX() const;
        [[nodiscard]] double MouseDY() const;
        [[nodiscard]] double MouseDWheel() const;

        [[nodiscard]] glm::vec2 MousePosition() const;

        [[nodiscard]] bool WindowResized() const;
        [[nodiscard]] vd::Dimension WindowSize() const;

        void WindowResizeCallback(vd::Dimension dimension);
        void KeyboardCallback(Key::Code key, Action::Code action);
        void MouseCallback(double x, double y);
        void MouseClickCallback(Button::Code button, Action::Code action);
        void MouseScrollCallback(double x, double y);

    private:
        const int kKeyCount = 350;
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

#endif //VDGE_EVENT_HPP