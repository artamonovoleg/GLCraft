#include "Keyboard.hpp"
#include "Engine.hpp"

Keyboard::Keyboard()
{
    std::fill(m_Keys, m_Keys + GLFW_KEY_LAST, PressState::None);
    glfwSetKeyCallback(*Engine::GetWindow(), [](GLFWwindow*, int key, int scancode, int action, int mods)
    {
        static auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        switch (action)
        {
            case GLFW_PRESS:
                keyboard.m_Keys[key] = PressState::Press;
                break;
            case GLFW_RELEASE:
                keyboard.m_Keys[key] = PressState::Release;
                break;
            default:
                break;
        }
    });
}