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