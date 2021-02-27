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
        }

        ~VertexBuffer()
        {
            glDeleteBuffers(1, &m_ID);
        }

        void Bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        }

        void Unbind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Map(void* data, size_t size)
        {
            Bind();
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }

        void SetLayout(GLuint index, GLint size, GLsizei stride, const void *pointer)
        {
            glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
            glEnableVertexAttribArray(index);
        }
};
