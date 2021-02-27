#pragma once

#include <glad/glad.h>

class VertexArray
{
    private:
        unsigned int m_ID;
    public:
        VertexArray()
        {
            glGenVertexArrays(1, &m_ID);
        }

        ~VertexArray()
        {
            glDeleteVertexArrays(1, &m_ID);
        }

        void Bind()
        {
            glBindVertexArray(m_ID);
        }

        void Unbind()
        {
            glBindVertexArray(0);
        }
};