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

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

class World
{
    private:
        std::shared_ptr<Shader>     m_Shader;
        std::shared_ptr<Texture>    m_Texture;

        const Camera& m_Camera;
        std::unordered_map<glm::ivec3, Chunk> m_Data;
    public:
        World(const Camera& camera)
            : m_Camera(camera)
        {
            m_Shader = std::make_shared<Shader>("../shaders/vert.glsl", "../shaders/frag.glsl");
            m_Texture = std::make_shared<Texture>("../assets/textures/atlas.png");
            m_Shader->SetInt("texture1", 0);

            for (int iz = 0; iz < 3; iz++)
            {
                for (int ix = 0; ix < 3; ix++)
                    m_Data.insert({{ ix, 0, iz }, Chunk({ ix * 16, 0, iz * 16 })});
            }
        }

        // Some test functions
        Chunk* GetChunk(const glm::vec3& position)
        {
            return &m_Data.at(GlobalToChunkPosition(position));
        }
        
        Voxel* Get(int ix, int iy, int iz)
        {
            auto* ch = GetChunk({ix, iy, iz});
            return &ch->m_ChunkData.At(GlobalVoxelToLocal({ ix, iy, iz }));
        }

        void Draw()
        {
            m_Texture->Bind();
            m_Shader->Bind();
            m_Shader->SetMat4("u_Model", glm::mat4(1.0));
            m_Shader->SetMat4("u_View", m_Camera.GetViewMatrix());
            m_Shader->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());

            for (auto& p : m_Data)
                p.second.Draw();
        }

        Voxel* Raycast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
        {
            float px = a.x;
            float py = a.y;
            float pz = a.z;
        
            float dx = dir.x;
            float dy = dir.y;
            float dz = dir.z;
        
            float t = 0.0f;
            int ix = floor(px);
            int iy = floor(py);
            int iz = floor(pz);
        
            float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
            float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
            float stepz = (dz > 0.0f) ? 1.0f : -1.0f;
        
            float infinity = std::numeric_limits<float>::infinity();
        
            float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
            float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
            float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);
        
            float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
            float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
            float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);
        
            float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
            float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
            float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;
        
            int steppedIndex = -1;
        
            while (t <= maxDist)
            {
                
                Voxel* voxel = Get(ix, iy, iz);

                if (voxel == nullptr || voxel->id)
                {
                    end.x = px + t * dx;
                    end.y = py + t * dy;
                    end.z = pz + t * dz;
        
                    iend.x = ix;
                    iend.y = iy;
                    iend.z = iz;
        
                    norm.x = norm.y = norm.z = 0.0f;
                    if (steppedIndex == 0) norm.x = -stepx;
                    if (steppedIndex == 1) norm.y = -stepy;
                    if (steppedIndex == 2) norm.z = -stepz;
                    return voxel;
                }
                if (txMax < tyMax) {
                    if (txMax < tzMax) {
                        ix += stepx;
                        t = txMax;
                        txMax += txDelta;
                        steppedIndex = 0;
                    } else {
                        iz += stepz;
                        t = tzMax;
                        tzMax += tzDelta;
                        steppedIndex = 2;
                    }
                } else {
                    if (tyMax < tzMax) {
                        iy += stepy;
                        t = tyMax;
                        tyMax += tyDelta;
                        steppedIndex = 1;
                    } else {
                        iz += stepz;
                        t = tzMax;
                        tzMax += tzDelta;
                        steppedIndex = 2;
                    }
                }
            }
            iend.x = ix;
            iend.y = iy;
            iend.z = iz;
        
            end.x = px + t * dx;
            end.y = py + t * dy;
            end.z = pz + t * dz;
            norm.x = norm.y = norm.z = 0.0f;

            return nullptr;
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

            if (keyboard.GetKeyDown(GLFW_KEY_X))
            {
                // auto* ch = w.GetChunk(camera.GetPosition());
                // auto breakPos = GlobalVoxelToLocal(GlobalToVoxel(camera.GetPosition()));
                // breakPos.y = 127;
                glm::vec3 end;
                glm::vec3 norm;
                glm::vec3 iend;
                auto* voxel = w.Raycast(camera.GetPosition(), camera.GetViewDirection(), 5.0f, end, norm, iend);
                // ch->m_ChunkData.At(breakPos).id = VoxelID::Air;
                if (voxel != nullptr)
                    voxel->id = VoxelID::Air;
                auto* ch = w.GetChunk(camera.GetPosition());
                ch->m_Vertices.clear();
                ch->m_Indices.clear();
                ch->GenerateMesh();
            }

            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            Engine::GetWindow()->SwapBuffers();
        }
    }
    Engine::Shutdown();
}