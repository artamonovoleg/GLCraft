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
#include "Texture.hpp"
#include "Camera.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    Camera camera({ 0.0f, 0.0f, 3.0f });
    
    Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    Texture mainTexture("../assets/textures/atlas.png");

    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;

    Chunk chunk ({ 0, -256, 0 });

    va.Bind();

    vb.Bind();
    vb.Map(chunk.GetVertices().data(), chunk.GetVerticesSize());
    vb.SetLayout();

    ib.Bind();
    ib.Map(chunk.GetIndices().data(), chunk.GetIndicesSize());

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
    auto& mouse = Engine::GetEventSystem()->GetMouse();

    mainTexture.Bind();
    mainShader.Bind();
    mainShader.SetInt("texture1", 0);


    auto& window = Engine::GetWindow();
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

    mainShader.SetMat4("u_Projection", proj);

    va.Bind();

    glEnable(GL_DEPTH_TEST);

    while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.Update(deltaTime);

        glClearColor(0.2, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        mainTexture.Bind();
        mainShader.Bind();
        mainShader.SetMat4("u_View", camera.GetViewMatrix());
        
        va.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);

        Engine::GetWindow()->SwapBuffers();
        Engine::GetEventSystem()->Process();
    }
}