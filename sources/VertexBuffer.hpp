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

        // Todo: Write it later
        void SetLayout()
        {
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }
};
