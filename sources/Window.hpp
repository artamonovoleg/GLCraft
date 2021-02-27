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
        Window(const WindowProps& props);
        ~Window();

        bool ShouldClose();
        void SwapBuffers();
        
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        
        operator GLFWwindow* () const { return m_Window; }
};