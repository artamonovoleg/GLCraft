#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Chunk.hpp"

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });

    Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");

    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;

    Chunk chunk ({ 0, 0, 0 });

    va.Bind();

    vb.Bind();
    vb.Map(chunk.GetVertices().data(), chunk.GetVerticesSize());
    vb.SetLayout();

    ib.Bind();
    ib.Map(chunk.GetIndices().data(), chunk.GetIndicesSize());
    

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