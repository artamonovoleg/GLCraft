#pragma once

#include <cstdint>
#include <glad/glad.h>

class IndexBuffer
{
    private:
        unsigned int m_ID;
        size_t m_Count = 0;
    public:
        IndexBuffer()
        {
            glGenBuffers(1, &m_ID);
        }

        ~IndexBuffer()
        {
            glDeleteBuffers(1, &m_ID);
        }

        void Bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        }

        void Unbind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void Map(void* data, size_t size)
        {
            Bind();
            m_Count = size / sizeof(unsigned int);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }

        size_t GetIndicesCount() { return m_Count; }
};