#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "EventSystem.hpp"
#include "Engine.hpp"
#include "Window.hpp"
#include <iostream>

EventSystem::EventSystem()
{
    SetupCallbacks(*Engine::GetWindow());
}

void EventSystem::SetupCallbacks(GLFWwindow* pWindow)
{
    glfwSetKeyCallback(pWindow, [](GLFWwindow*, int key, int scancode, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
                Engine::GetEventSystem()->GetKeyboard().m_Keys[key] = PressState::Press;
                break;
            case GLFW_RELEASE:
                Engine::GetEventSystem()->GetKeyboard().m_Keys[key] = PressState::Release;
                break;
            default:
                break;
        }
    });

    glfwSetMouseButtonCallback(pWindow, [](GLFWwindow*, int button, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
                Engine::GetEventSystem()->GetMouse().m_Buttons[button] = PressState::Press;
                break;
            case GLFW_RELEASE:
                Engine::GetEventSystem()->GetMouse().m_Buttons[button] = PressState::Release;
                break;
            default:
                break;
        }
    });

    glfwSetCursorPosCallback(pWindow, [](GLFWwindow*, double xpos, double ypos)
    {
        Engine::GetEventSystem()->GetMouse().m_Position = glm::vec3(static_cast<float>(xpos), static_cast<float>(ypos), 0);
    });

    glfwSetWindowSizeCallback(pWindow, [](GLFWwindow*, int width, int height)
    {
        Engine::GetWindow()->m_Width = width;
        Engine::GetWindow()->m_Height = height;
        glViewport(0, 0, width, height);
    });
}

void EventSystem::Process()
{
    glfwPollEvents();
}