#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include "PressState.hpp"

class Mouse
{
    private:
        mutable PressState  m_Buttons[3];
        glm::vec3   m_Position;

        mutable float m_OffsetX = 0.0f;
        mutable float m_OffsetY = 0.0f;
    public:
        Mouse();
                
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
        float GetOffsetX() const 
        { 
            float ret = m_OffsetX;
            m_OffsetX = 0.0f;
            return ret; 
        }
        
        float GetOffsetY() const 
        { 
            float ret = m_OffsetY;
            m_OffsetY = 0.0f;
            return ret; 
        }
};