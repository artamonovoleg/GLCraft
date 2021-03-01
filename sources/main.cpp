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
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    Engine::Init({ 1920, 1080, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 0.0f, 0.0f, 1.0f });
        
        Skybox skybox({     "../assets/skybox/right.jpg",
                            "../assets/skybox/left.jpg",
                            "../assets/skybox/top.jpg",
                            "../assets/skybox/bottom.jpg",
                            "../assets/skybox/front.jpg",
                            "../assets/skybox/back.jpg" });
                            
        Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
        Texture mainTexture("../assets/textures/atlas.png");

        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;

        Chunk chunk ( camera.Position - glm::vec3(8, 256, 8));

        va.Bind();

        vb.Bind();
        vb.Map(chunk.GetVertices().data(), chunk.GetVerticesSize());
        vb.SetLayout();

        ib.Bind();
        ib.Map(chunk.GetIndices().data(), chunk.GetIndicesSize());

        va.Unbind();

        mainTexture.Bind();
        mainShader.Bind();
        mainShader.SetInt("texture1", 0);

        mainShader.SetMat4("u_Projection", proj);

        glEnable(GL_DEPTH_TEST);

        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
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

            if (keyboard.GetKeyDown(GLFW_KEY_C))
                chunk.PrintBlockType({ 0, 256, 0 });
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}