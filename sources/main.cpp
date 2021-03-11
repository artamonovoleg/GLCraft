#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include "Mesh.hpp"
#include "MeshBuilder.hpp"
#include "Tests.hpp"

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 0, 0, 0 });
        
        Skybox skybox("../assets/skybox/", "jpg");

        Crosshair crosshair;

        Texture tex("../assets/textures/atlas.png");
        Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
        shader.SetInt("texture", 0);

        RunTest();
        
        Mesh mesh;
        ChunkManager manager(camera);
        // manager.AddChunk({ 0, 0, -1 });
        manager.AddChunk(ToChunkPosition({ 0, 0, -1 })).QSetVoxel({ 1, 1, 0 }, Voxel::Grass);
        manager.SetVoxel(LocalVoxelToGlobal({ 0, 0, -1 }, { 0, 0, 0 }), Voxel::Air);
        MeshBuilder meshBuilder(manager);

        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            Engine::GetEventSystem()->Process();

            // timing
            static float deltaTime = 0.0f;
            static float lastFrame = 0.0f;
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);
            // manager.Process();
            // meshBuilder.Process();

            window->Clear();

            shader.Bind();
            shader.SetMat4("u_Projection", camera.GetProjectionMatrix());
            shader.SetMat4("u_View", camera.GetViewMatrix());
            shader.SetMat4("u_Model", glm::mat4(1.0f));

            for (const auto& mesh : meshBuilder.GetMeshes())
                mesh.second.Draw();
            
            if (keyboard.GetKeyDown(GLFW_KEY_F))
            {
                manager.SetVoxel(LocalVoxelToGlobal({ 0, 0, -1 }, { 0, 0, 0 }), Voxel::Air);
            }
            
            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}