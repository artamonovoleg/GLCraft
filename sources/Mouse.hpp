#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include "PressState.hpp"

class Mouse
{
    private:
        friend class EventSystem;

        mutable PressState  m_Buttons[3];
        mutable glm::vec3   m_Position;
    public:
        Mouse()
        {
            std::fill(m_Buttons, m_Buttons + 3, PressState::None);
        }
        
        bool GetButton(int button) const { return m_Buttons[button] == PressState::Press; }

        bool GetButtonDown(int button) const 
        {
            bool res = false;
            if (m_Buttons[button] == PressState::Press)
            {
                res = true;
                m_Buttons[button] = PressState::Hold;
            }
            return res;
        }

        bool GetButtonUp(int button) const 
        {
            bool res = false;
            if (m_Buttons[button] == PressState::Release)
            {
                res = true;
                m_Buttons[button] = PressState::None;
            }
            return res;
        }

        const glm::vec3& GetPosition() const { return m_Position; }
};