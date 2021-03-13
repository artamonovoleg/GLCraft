#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include "PressState.hpp"

class Keyboard
{
    private:
        mutable PressState m_Keys[GLFW_KEY_LAST];
    public:
        Keyboard();

        bool GetKeyDown(int key) const
        {
            bool res = false;

            if (m_Keys[key] == PressState::Press)
            {
                res = true;
                m_Keys[key] = PressState::Hold;
            }

            return res;
        }

        bool GetKeyUp(int key) const
        {
            bool res = false;

            if (m_Keys[key] == PressState::Release)
            {
                res = true;
                m_Keys[key] = PressState::None;
            }

            return res;
        }

        bool GetKey(int key) const
        {
            return (m_Keys[key] == PressState::Hold || m_Keys[key] == PressState::Press);
        }
};