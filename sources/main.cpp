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

struct RaycastResult
{
    VoxelPosition norm;
    VoxelPosition end;
};

std::optional<RaycastResult> Raycast(const ChunkManager& manager, const glm::vec3& startPoint, const glm::vec3& direction, float range)
{
    auto nDirection = glm::normalize(direction);
    auto endPoint = startPoint + nDirection * range;
    auto startVoxel = ToVoxelPosition(startPoint);

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
    int steppedIndex = -1;

    while (++t < range * 3) 
    {
        if (manager.GetVoxel(startVoxel) != Voxel::Air)
        {
            RaycastResult res;
            res.end = startVoxel;
            res.norm.x = res.norm.y = res.norm.z = 0.0f;
            if (steppedIndex == 0) res.norm.x = -stepX;
            if (steppedIndex == 1) res.norm.y = -stepY;
            if (steppedIndex == 2) res.norm.z = -stepZ;
            return res;
        }

        if (tMaxX < tMaxY) 
        {
            if (tMaxX < tMaxZ) 
            {
                startVoxel.x += stepX;
                tMaxX += tDeltaX;
                steppedIndex = 0;
            }
            else 
            {
                startVoxel.z += stepZ;
                tMaxZ += tDeltaZ;
                steppedIndex = 2;
            }
        }
        else 
        {
            if (tMaxY < tMaxZ) 
            {
                startVoxel.y += stepY;
                tMaxY += tDeltaY;
                steppedIndex = 1;
            }
            else 
            {
                startVoxel.z += stepZ;
                tMaxZ += tDeltaZ;
                steppedIndex = 2;
            }
        }
        if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
            break;
    }
    return {};
};

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();

        Camera camera({ 0, 1, 0 });
        
        Skybox skybox("../assets/skybox_sun/", "bmp");

        Crosshair crosshair;

        Texture tex("../assets/textures/atlas.png");
        Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
        shader.SetInt("texture", 0);

        Mesh mesh;
        ChunkManager manager(camera);
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
            meshBuilder.Process();
            manager.Process();

            window->Clear();

            shader.Bind();
            shader.SetMat4("u_Projection", camera.GetProjectionMatrix());
            shader.SetMat4("u_View", camera.GetViewMatrix());
            shader.SetMat4("u_Model", glm::mat4(1.0f));

            for (const auto& mesh : meshBuilder.GetMeshes())
                mesh.second.Draw();
            auto res = Raycast(manager, camera.GetPosition(), camera.GetViewDirection(), 10.0f);
            if (res.has_value())
            {
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    manager.SetVoxel(res->end, Voxel::Air);
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                    manager.SetVoxel(res->end + res->norm, Voxel::Glass);
            }
            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}