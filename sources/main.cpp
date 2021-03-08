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

std::ostream& operator<< (std::ostream& os, const glm::vec3& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 tex;
};

class ChunkManager
{
    private:
    public:
        const Voxel& GetVoxel(const VoxelPosition& position) const
        {
            // return GetChunk(position).QuickGetVoxel(position);
        }
};

class Chunk
{
    private:
        const ChunkManager& m_ChunkManager;

        std::array<Voxel, ChunkSide * ChunkSide * ChunkHeight> m_Data;

        VoxelPosition m_Position;
    public:
        Chunk(const ChunkManager& chunkManager)
            : m_ChunkManager(chunkManager) {}

        bool PositionInBounds(const VoxelPosition& position) const
        {
            return (position.x >= 0 && position.z >= 0 && position.y >= 0 && position.x < ChunkSide && position.y < ChunkHeight && position.z < ChunkSide);
        }

        /// Unsafe function without check is position in bounds. Returns voxel by local chunk coords
        const Voxel& QuickGetVoxel(const VoxelPosition& position) const
        {
            return m_Data.at(position.x + ChunkSide * ChunkSide * position.y + ChunkSide * position.z);
        }
        
        /// Safe function. If chunk not in bounds return from neighbour chunk
        const Voxel& GetVoxel(const VoxelPosition& position) const
        {
            if (PositionInBounds(position)) return QuickGetVoxel(position);

            return m_ChunkManager.GetVoxel(LocalToGlobalVoxel(m_Position, position));
        }
};

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
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            Engine::GetWindow()->SwapBuffers();
        }
    }
    Engine::Shutdown();

    return 0;
}