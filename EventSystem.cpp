#include <GLFW/glfw3.h>
#include "EventSystem.hpp"

EventSystem::EventSystem()
{
}

void EventSystem::Process()
{
    glfwPollEvents();
}