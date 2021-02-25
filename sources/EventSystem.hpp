#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"

class EventSystem
{
    private:
        Keyboard    m_Keyboard;
        Mouse       m_Mouse;
        void SetupCallbacks(GLFWwindow* pWindow);
    public:
        EventSystem();

        void Process();

        const Keyboard& GetKeyboard()   const { return m_Keyboard; }
        const Mouse&    GetMouse()      const { return m_Mouse; }
};