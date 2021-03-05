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
#include "Crosshair.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 8.0f, 129.0f, 8.0f });
        
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

        
        Crosshair crosshair;

        glm::ivec3 norm;
        glm::vec3 end;
        glm::ivec3 iend;

        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            crosshair.Draw();

            mainTexture.Bind();
            mainShader.Bind();
            mainShader.SetMat4("u_Model", glm::mat4(1.0));
            mainShader.SetMat4("u_View", camera.GetViewMatrix());
            mainShader.SetMat4("u_Projection", proj);
            
            chunk.Draw();

            auto castres = chunk.RayCast(camera.Position, camera.Front, 5.0f, end, norm, iend);
            if (castres.has_value())
            {
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    castres->chunk->Set(castres->blockPosition, BlockType::Air);
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                    castres->chunk->Set(castres->blockPosition + castres->normal, BlockType::Grass);
            }
                
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}