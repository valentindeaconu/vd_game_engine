//
// Created by Vali on 11/12/2020.
//

#include "EventHandler.hpp"

namespace vd::event {
    EventHandler::EventHandler()
        : m_MouseSensitivity(0.03f)
        , m_KeysStatus(kKeyCount)
        , m_ButtonsStatus(kButtonCount)
    {
    }

    EventHandler::~EventHandler() = default;

    bool EventHandler::KeyDown(int key) const {
        return m_KeysStatus[key] == eKeyPressed || m_KeysStatus[key] == eKeyPostPressed;
    }

    bool EventHandler::KeyReleased(int key) const {
        return m_KeysStatus[key] == eKeyReleased || m_KeysStatus[key] == eKeyPostReleased;
    }

    bool EventHandler::KeyHolding(int key) const {
        return m_KeysStatus[key] == eKeyHolding;
    }

    bool EventHandler::ButtonDown(int button) const {
        return m_ButtonsStatus[button] == eButtonPressed || m_ButtonsStatus[button] == eButtonPostPressed;
    }

    bool EventHandler::ButtonReleased(int button) const {
        return m_ButtonsStatus[button] == eButtonReleased || m_ButtonsStatus[button] == eButtonPostReleased;
    }

    bool EventHandler::ButtonHolding(int button) const {
        return m_ButtonsStatus[button] == eButtonHolding;
    }

    float& EventHandler::MouseSensitivity() {
        return m_MouseSensitivity;
    }

    bool EventHandler::MouseMoved() const {
        return m_MouseMovement.status == eMovePreUpdated || m_MouseMovement.status == eMovePostUpdated;
    }

    bool EventHandler::MouseScrolled() const {
        return m_MouseScroll.status == eWheelPreUpdated || m_MouseScroll.status == eWheelPostUpdated;
    }

    double EventHandler::MouseDX() const {
        return m_MouseMovement.offset.x;
    }

    double EventHandler::MouseDY() const {
        return m_MouseMovement.offset.y;
    }

    double EventHandler::MouseDWheel() const {
        return m_MouseScroll.offset.y;
    }

    glm::vec2 EventHandler::MousePosition() const {
        return m_MouseMovement.curr;
    }

    bool EventHandler::WindowResized() const {
        return m_WindowInfo.status == eWindowPreResize || m_WindowInfo.status == eWindowPostResize;
    }

    vd::Dimension EventHandler::WindowSize() const {
        return m_WindowInfo.info;
    }

    void EventHandler::WindowResizeCallback(GLFWwindow* window, int width, int height) {
        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

        this->m_WindowInfo.info = vd::Dimension(uint64_t(glm::abs(bufferWidth)), uint64_t(glm::abs(bufferHeight)));

        this->m_WindowInfo.status = eWindowPreResize;
    }

    void EventHandler::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key >= 0 && key < 1024) {
            if (action == GLFW_PRESS) {
                if (this->m_KeysStatus[key] != eKeyHolding)
                    this->m_KeysStatus[key] = eKeyPressed;
            } else if (action == GLFW_RELEASE) {
                this->m_KeysStatus[key] = eKeyReleased;
            }
        }
    }

    void EventHandler::MouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
        this->m_MouseMovement.status = eMovePreUpdated;
        this->m_MouseMovement.offset.x = float((x_pos - this->m_MouseMovement.curr.x) * this->m_MouseSensitivity);
        this->m_MouseMovement.offset.y = float((this->m_MouseMovement.curr.y - y_pos) * this->m_MouseSensitivity);

        this->m_MouseMovement.curr = glm::vec2(float(x_pos), float(y_pos));
    }

    void EventHandler::MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button >= 0 && button < 8) {
            if (action == GLFW_PRESS) {
                if (this->m_ButtonsStatus[button] != eButtonHolding)
                    this->m_ButtonsStatus[button] = eButtonPressed;
            } else if (action == GLFW_RELEASE) {
                this->m_ButtonsStatus[button] = eButtonReleased;
            }
        }
    }

    void EventHandler::MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
        this->m_MouseScroll.status = eWheelPreUpdated;
        this->m_MouseScroll.offset = glm::vec2(float(x_offset), float(y_offset));
    }

    EventHandlerManager::EventHandlerManager() {
        m_EventHandlerPtr = std::make_shared<EventHandler>();

        // TODO: Set this in a property file
        m_EventHandlerPtr->MouseSensitivity() = 0.03f;

        vd::ObjectOfType<EventHandler>::Provide(m_EventHandlerPtr);
    }

    EventHandlerManager::~EventHandlerManager() = default;

    void EventHandlerManager::Init() {

    }

    void EventHandlerManager::Update() {
        for (size_t i = 0; i < m_EventHandlerPtr->kKeyCount; ++i) {
            switch (m_EventHandlerPtr->m_KeysStatus[i]) {
                case EventHandler::KeyStatus::eKeyPressed:
                    m_EventHandlerPtr->m_KeysStatus[i] = EventHandler::KeyStatus::eKeyPostPressed;
                    break;
                case EventHandler::KeyStatus::eKeyPostPressed:
                    m_EventHandlerPtr->m_KeysStatus[i] = EventHandler::KeyStatus::eKeyHolding;
                    break;
                case EventHandler::KeyStatus::eKeyReleased:
                    m_EventHandlerPtr->m_KeysStatus[i] = EventHandler::KeyStatus::eKeyPostReleased;
                    break;
                case EventHandler::KeyStatus::eKeyPostReleased:
                    m_EventHandlerPtr->m_KeysStatus[i] = EventHandler::KeyStatus::eKeyFree;
                    break;
                default:
                    break;
            }
        }

        for (size_t i = 0; i < m_EventHandlerPtr->kButtonCount; ++i) {
            switch (m_EventHandlerPtr->m_ButtonsStatus[i]) {
                case EventHandler::ButtonStatus::eButtonPressed:
                    m_EventHandlerPtr->m_ButtonsStatus[i] = EventHandler::ButtonStatus::eButtonPostPressed;
                    break;
                case EventHandler::ButtonStatus::eButtonPostPressed:
                    m_EventHandlerPtr->m_ButtonsStatus[i] = EventHandler::ButtonStatus::eButtonHolding;
                    break;
                case EventHandler::ButtonStatus::eButtonReleased:
                    m_EventHandlerPtr->m_ButtonsStatus[i] = EventHandler::ButtonStatus::eButtonPostReleased;
                    break;
                case EventHandler::ButtonStatus::eButtonPostReleased:
                    m_EventHandlerPtr->m_ButtonsStatus[i] = EventHandler::ButtonStatus::eButtonFree;
                    break;
                default:
                    break;
            }
        }

        if (m_EventHandlerPtr->m_MouseMovement.status == EventHandler::MouseMoveStatus::eMovePreUpdated) {
            m_EventHandlerPtr->m_MouseMovement.status = EventHandler::MouseMoveStatus::eMovePostUpdated;
        } else if (m_EventHandlerPtr->m_MouseMovement.status == EventHandler::MouseMoveStatus::eMovePostUpdated) {
            m_EventHandlerPtr->m_MouseMovement.status = EventHandler::MouseMoveStatus::eMoveFree;
        }

        if (m_EventHandlerPtr->m_MouseScroll.status == EventHandler::MouseWheelStatus::eWheelPreUpdated) {
            m_EventHandlerPtr->m_MouseScroll.status = EventHandler::MouseWheelStatus::eWheelPostUpdated;
        } else if (m_EventHandlerPtr->m_MouseScroll.status == EventHandler::MouseWheelStatus::eWheelPostUpdated) {
            m_EventHandlerPtr->m_MouseScroll.status = EventHandler::MouseWheelStatus::eWheelFree;
        }

        if (m_EventHandlerPtr->m_WindowInfo.status == EventHandler::WindowStatus::eWindowPreResize) {
            m_EventHandlerPtr->m_WindowInfo.status = EventHandler::WindowStatus::eWindowPostResize;
        } else if (m_EventHandlerPtr->m_WindowInfo.status == EventHandler::WindowStatus::eWindowPostResize) {
            m_EventHandlerPtr->m_WindowInfo.status = EventHandler::WindowStatus::eWindowFree;
        }
    }

    void EventHandlerManager::CleanUp() {

    }
}

