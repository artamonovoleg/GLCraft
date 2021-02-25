#include "Engine.hpp"

std::unique_ptr<Window>         m_Window = nullptr;
std::unique_ptr<EventSystem>    m_EventSystem = nullptr;

void Engine::Init(const WindowProps& props)
{
    glfwInit();
    m_Window        = std::make_unique<Window>(props);
    m_EventSystem   = std::make_unique<EventSystem>();
}

void Engine::Shutdown()
{
    glfwTerminate();
}

std::unique_ptr<Window>&         Engine::GetWindow() { return m_Window; }
std::unique_ptr<EventSystem>&    Engine::GetEventSystem() { return m_EventSystem; }