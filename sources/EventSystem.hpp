#pragma once

#include "Keyboard.hpp"

class EventSystem
{
    private:
        Keyboard m_Keyboard;

        void SetupCallbacks(GLFWwindow* pWindow);
    public:
        EventSystem();

        void Process();

        const Keyboard& GetKeyboard() const { return m_Keyboard; }
};