#include <memory>
#include <unordered_map>
#include <array>
#include <iostream>
#include <cmath>
#include <initializer_list>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"
#include "Voxel.hpp"
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "ChunkManager.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "MeshBuilder.hpp"

struct RaycastResult
{
    VoxelPosition end;
    VoxelPosition norm;
};

std::optional<RaycastResult> Raycast(ChunkManager& chm, const glm::vec3& startPoint, const glm::vec3& direction, float range)
{
      // Ensures passed direction is normalized
    auto nDirection = glm::normalize(direction);
    auto endPoint = startPoint + nDirection * range;
    auto startVoxel = GlobalToVoxel(startPoint);

    // +1, -1, or 0
    int stepX = (nDirection.x > 0) ? 1 : ((nDirection.x < 0) ? -1 : 0);
    int stepY = (nDirection.y > 0) ? 1 : ((nDirection.y < 0) ? -1 : 0);
    int stepZ = (nDirection.z > 0) ? 1 : ((nDirection.z < 0) ? -1 : 0);

    float tDeltaX =
        (stepX != 0) ? fmin(stepX / (endPoint.x - startPoint.x), FLT_MAX) : FLT_MAX;
    float tDeltaY =
        (stepY != 0) ? fmin(stepY / (endPoint.y - startPoint.y), FLT_MAX) : FLT_MAX;
    float tDeltaZ =
        (stepZ != 0) ? fmin(stepZ / (endPoint.z - startPoint.z), FLT_MAX) : FLT_MAX;

    float tMaxX = (stepX > 0.0f) ? tDeltaX * (1.0f - startPoint.x + startVoxel.x)
                                 : tDeltaX * (startPoint.x - startVoxel.x);
    float tMaxY = (stepY > 0.0f) ? tDeltaY * (1.0f - startPoint.y + startVoxel.y)
                                 : tDeltaY * (startPoint.y - startVoxel.y);
    float tMaxZ = (stepZ > 0.0f) ? tDeltaZ * (1.0f - startPoint.z + startVoxel.z)
                                 : tDeltaZ * (startPoint.z - startVoxel.z);


    int t = 0;
    while (++t < range * 3) 
    {
        if (chm.GetVoxel(startVoxel).type != VoxelType::Air)
        {
            return RaycastResult{ startVoxel, { 0, 0, 0 } };
        }

        if (tMaxX < tMaxY) 
        {
            if (tMaxX < tMaxZ) 
            {
                startVoxel.x += stepX;
                tMaxX += tDeltaX;
            }
            else 
            {
                startVoxel.z += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
        else 
        {
            if (tMaxY < tMaxZ) 
            {
                startVoxel.y += stepY;
                tMaxY += tDeltaY;
            }
            else 
            {
                startVoxel.z += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
        if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
            break;
    }
    return {};
}

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 0, 16, 0 });
        
        Skybox skybox("../assets/skybox/", "jpg");

        Crosshair crosshair;

        Texture tex("../assets/textures/atlas.png");
        Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
        shader.SetInt("texture", 0);

        Mesh mesh;

        ChunkManager m(camera);
        MeshBuilder mb(m);
        
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
            
            for (const auto& mesh : mb.GetMeshes())
            {
                mesh.second.va->Bind();
                glDrawElements(GL_TRIANGLES, mesh.second.ib->GetCount(), GL_UNSIGNED_INT, 0);
            }

            auto res = Raycast(m, camera.GetPosition(), camera.GetViewDirection(), 5.0f);

            if (res.has_value() && mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            {
                m.SetVoxel(res->end, VoxelType::Air);
            }

            mb.Update();

            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}