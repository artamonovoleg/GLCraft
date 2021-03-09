#pragma once

#include <glad/glad.h>

class VertexBuffersLayout
{
    private:
        mutable int m_Count = 0;
        unsigned int m_Offset = 0;
    public:
        VertexBuffersLayout() = default;
        ~VertexBuffersLayout() = default;

        template<typename T>
        void Push(unsigned int amount, unsigned int stride)
        {
            glVertexAttribPointer(m_Count, amount, GL_FLOAT, GL_FALSE, stride * sizeof(T), (void *)(m_Offset * sizeof(T)));
            glEnableVertexAttribArray(m_Count);
            m_Offset += amount;
            m_Count++;
        }
};