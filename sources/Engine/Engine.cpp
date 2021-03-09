#include <glad/glad.h>
#include "Engine.hpp"

std::unique_ptr<Window>         m_Window = nullptr;
std::unique_ptr<EventSystem>    m_EventSystem = nullptr;

void Engine::Init(const WindowProps& props)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window        = std::make_unique<Window>(props);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    m_EventSystem   = std::make_unique<EventSystem>();
    glfwSetInputMode(*m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::Shutdown()
{
    glfwTerminate();
}

std::unique_ptr<Window>&         Engine::GetWindow() { return m_Window; }
std::unique_ptr<EventSystem>&    Engine::GetEventSystem() { return m_EventSystem; }