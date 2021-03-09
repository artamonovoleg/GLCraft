#include <vector>
#include <iostream>
#include <glm/glm.hpp>
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
#include "Voxel.hpp"
#include "Constants.hpp"
#include "Mesh.hpp"

using VoxelPosition = glm::ivec3;
VoxelDataManager voxDM;

void PushIndices(std::vector<unsigned int>& indices)
{
    uint32_t m;
    if (!indices.empty())
        m = indices.back() + 1;
    else
        m = 0;
    indices.insert(indices.end(), { 0 + m, 1 + m, 2 + m, 2 + m, 1 + m, 3 + m } );
}

void PushFace(Mesh& mesh, const VoxelPosition& position, Voxel voxel, Face face)
{
    auto& vertices  = voxDM.GetVertices(face);
    auto [u, v]     = voxDM.GetUV(voxel, face);

    mesh.vertices.insert(mesh.vertices.end(),
    {
        { vertices.at(0) + position, { u, v + UVSize } },
        { vertices.at(1) + position, { u + UVSize, v + UVSize } },
        { vertices.at(2) + position, { u, v } },
        { vertices.at(3) + position, { u + UVSize, v } }
    });

    PushIndices(mesh.indices);
}

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

        Mesh mesh;
        VoxelPosition position(0, 1, 0);

        PushFace(mesh, position, Voxel::Glass, Face::Left);
        PushFace(mesh, position, Voxel::Glass, Face::Right);
        PushFace(mesh, position, Voxel::Glass, Face::Front);
        PushFace(mesh, position, Voxel::Glass, Face::Back);
        PushFace(mesh, position, Voxel::Glass, Face::Bottom);
        
        mesh.Load();

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

            window->Clear();

            shader.Bind();
            shader.SetMat4("u_Projection", camera.GetProjectionMatrix());
            shader.SetMat4("u_View", camera.GetViewMatrix());
            shader.SetMat4("u_Model", glm::mat4(1.0f));
            mesh.Draw();

            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}