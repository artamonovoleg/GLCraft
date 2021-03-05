#pragma once

#include <cstdint>
#include <glad/glad.h>

class VertexBuffer
{
    private:
        unsigned int m_ID;
    public:
        VertexBuffer()
        {
            glGenBuffers(1, &m_ID);
            Bind();
        }

        ~VertexBuffer()
        {
            Unbind();
            glDeleteBuffers(1, &m_ID);
        }

        void Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        }

        void Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Map(void* data, size_t size)
        {
            Bind();
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }
};
