#include "Window.hpp"

Window::Window(const WindowProps& props)
    : m_Width(props.width), m_Height(props.height)
{
    m_Window = glfwCreateWindow(m_Width, m_Height, props.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}