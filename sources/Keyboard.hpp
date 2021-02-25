#pragma once

#include <array>
#include <GLFW/glfw3.h>

enum class PressState
{
    None,
    Press,
    Hold,
    Release
};

class EventSystem;

class Keyboard
{
    private:
        friend class EventSystem;
        mutable std::array<PressState, GLFW_KEY_LAST> m_Keys;
    public:
        Keyboard()
        {
            m_Keys.fill(PressState::None);
        }

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