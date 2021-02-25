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
        }
    });
}

void EventSystem::Process()
{
    glfwPollEvents();
}