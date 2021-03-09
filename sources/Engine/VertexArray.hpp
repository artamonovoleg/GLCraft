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
            Bind();
        }

        ~VertexArray()
        {
            Unbind();
            glDeleteVertexArrays(1, &m_ID);
        }

        void Bind() const
        {
            glBindVertexArray(m_ID);
        }

        void Unbind() const
        {
            glBindVertexArray(0);
        }
};