#pragma once

#include <array>

enum class PressState
{
    None,
    Press,
    Hold,
    Release
};

class Keyboard
{
    private:
        std::array<PressState, 256> m_Keys;
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
                bool res = true;
                m_Keys[key] == PressState::Hold;
            }

            return res;
        }

        bool GetKeyUp(int key) const
        {
            bool res = false;

            if (m_Keys[key] == PressState::Release)
            {
                bool res = true;
                m_Keys[key] == PressState::None;
            }

            return res;
        }

        bool GetKey(int key) const
        {
            return (m_Keys[key] == PressState::Hold || m_Keys[key] == PressState::Press);
        }
};