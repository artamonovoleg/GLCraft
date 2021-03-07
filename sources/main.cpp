#include <memory>
#include <unordered_map>
#include <iostream>
#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"
#include "Chunk.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"
#include "Coordinates.hpp"
#include "World.hpp"

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

        Camera camera({ 0, 129, 0 });
        
        Skybox skybox({     "../assets/skybox/right.jpg",
                            "../assets/skybox/left.jpg",
                            "../assets/skybox/top.jpg",
                            "../assets/skybox/bottom.jpg",
                            "../assets/skybox/front.jpg",
                            "../assets/skybox/back.jpg" });

        World w (camera);

        Crosshair crosshair;

        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            Engine::GetEventSystem()->Process();

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);

            Engine::GetWindow()->Clear();

            crosshair.Draw();
            w.Draw();

            glm::vec3 norm;
            glm::vec3 iend;
            auto* voxel = w.Raycast(camera.GetPosition(), camera.GetViewDirection(), 5.0f, norm, iend);

            if (voxel != nullptr)
            {
                bool hasUpdate = false;
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                {
                    voxel->id = VoxelID::Air;
                    hasUpdate = true;
                }
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                {
                    auto pos = iend + norm;
                    w.Get(pos.x, pos.y, pos.z)->id = VoxelID::Grass;
                    hasUpdate = true;
                }

                if (hasUpdate)
                {
                    auto* ch = w.GetChunk(iend);
                    ch->m_Vertices.clear();
                    ch->m_Indices.clear();
                    ch->GenerateMesh();
                }
            }

            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            Engine::GetWindow()->SwapBuffers();
        }
    }
    Engine::Shutdown();

    return 0;
}