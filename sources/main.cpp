#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"

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

        void SetVertices(void* data, size_t size)
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

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });

    Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    VertexArray va;
    VertexBuffer vb;
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    va.Bind();

    vb.Bind();
    vb.SetVertices(vertices, sizeof(vertices));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    vb.SetLayout(0, 3, 3 * sizeof(float), (void*)0);

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
    auto& mouse = Engine::GetEventSystem()->GetMouse();

    mainShader.Bind();
    va.Bind();

    while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
    {
        if (keyboard.GetKeyDown(GLFW_KEY_A))
            std::cout << "A down" << std::endl;
        if (keyboard.GetKeyUp(GLFW_KEY_D))
            std::cout << "D up" << std::endl;
        if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            std::cout << "Mouse left down" << std::endl;

        glClearColor(0.2, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        Engine::GetWindow()->SwapBuffers();
        Engine::GetEventSystem()->Process();
    }
}