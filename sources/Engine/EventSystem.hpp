#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"

class EventSystem
{
    private:
        mutable Keyboard    m_Keyboard;
        mutable Mouse       m_Mouse;
        void SetupCallbacks(GLFWwindow* pWindow);
    public:
        EventSystem();

        void Process();

        Keyboard& GetKeyboard()   const { return m_Keyboard; }
        Mouse&    GetMouse()      const { return m_Mouse; }
};