#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"
#include "Chunk.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float vertices [] = 
{   -0.03, 0.0, 
    0.03, 0.0, 
    
    0.0, 0.05, 
    0.0, -0.05 };

int main()
{
    Engine::Init({ 1920, 1080, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 8.0f, 256.0f, 8.0f });
        
        Skybox skybox({     "../assets/skybox/right.jpg",
                            "../assets/skybox/left.jpg",
                            "../assets/skybox/top.jpg",
                            "../assets/skybox/bottom.jpg",
                            "../assets/skybox/front.jpg",
                            "../assets/skybox/back.jpg" });
                            
        Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
        Texture mainTexture("../assets/textures/atlas.png");

        Chunk chunk ( { 0, 0, 0 } );

        mainTexture.Bind();
        mainShader.Bind();
        mainShader.SetInt("texture1", 0);

        mainShader.SetMat4("u_Projection", proj);

        glEnable(GL_DEPTH_TEST);
        
        VertexArray cva;
        VertexBuffer cvb;
        cvb.Map(vertices, sizeof(vertices));
        VertexBuffersLayout layout;
        layout.Push<float>(2, 2);
        Shader cshader("../shaders/crosshair.vert", "../shaders/crosshair.frag");
        
        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);

            glClearColor(0.2, 0.3, 0.4, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cshader.Bind();
            cva.Bind();
            glDrawArrays(GL_LINES, 0, 8);
            mainTexture.Bind();
            mainShader.Bind();
            mainShader.SetMat4("u_View", camera.GetViewMatrix());
            
            chunk.Draw();

            if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                chunk.Raycast(camera.Position, camera.Front, 10.0f);
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}