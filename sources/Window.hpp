#pragma once

#include <string>
#include <GLFW/glfw3.h>

struct WindowProps
{
    int width, height;
    std::string title;
};

class Window
{
    private:
        int m_Width, m_Height;
        GLFWwindow* m_Window;
    public:
        Window(const WindowProps& props)
            : m_Width(props.width), m_Height(props.height)
        {
            m_Window = glfwCreateWindow(m_Width, m_Height, props.title.c_str(), nullptr, nullptr);
        }

        ~Window()
        {
            glfwDestroyWindow(m_Window);
        }

        bool ShouldClose()
        {
            return glfwWindowShouldClose(m_Window);
        }

        operator GLFWwindow* () const { return m_Window; }
};