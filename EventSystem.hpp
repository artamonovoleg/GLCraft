#pragma once

#include "Keyboard.hpp"

class EventSystem
{
    private:
        Keyboard m_Keyboard;
    public:
        EventSystem();

        void Process();

        const Keyboard& GetKeyboard() const { return m_Keyboard; }
};