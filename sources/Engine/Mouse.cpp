#include "Mouse.hpp"
#include "Engine.hpp"

Mouse::Mouse()
{
    std::fill(m_Buttons, m_Buttons + 3, PressState::None);

    glfwSetMouseButtonCallback(*Engine::GetWindow(), [](GLFWwindow*, int button, int action, int mods)
    {
        static Mouse& mouse = Engine::GetEventSystem()->GetMouse();
        switch (action)
        {
            case GLFW_PRESS:
                mouse.m_Buttons[button] = PressState::Press;
                break;
            case GLFW_RELEASE:
                mouse.m_Buttons[button] = PressState::Release;
                break;
            default:
                break;
        }
    });

    glfwSetCursorPosCallback(*Engine::GetWindow(), [](GLFWwindow*, double xpos, double ypos)
    {
        static Mouse& mouse = Engine::GetEventSystem()->GetMouse();
        static bool firstMouse = true;
        static float lastX;
        static float lastY;

        mouse.m_Position = glm::vec3(static_cast<float>(xpos), static_cast<float>(ypos), 0);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        mouse.m_OffsetX = xpos - lastX;
        mouse.m_OffsetY = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 1.1f;
        mouse.m_OffsetX *= sensitivity;
        mouse.m_OffsetY *= sensitivity;
    });
}