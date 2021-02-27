#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

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
    IndexBuffer ib;

    va.Bind();

    vb.Bind();
    vb.Map(vertices, sizeof(vertices));

    ib.Bind();
    ib.Map(indices, sizeof(indices));
    
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

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);

        Engine::GetWindow()->SwapBuffers();
        Engine::GetEventSystem()->Process();
    }
}