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

            for (int iz = camera.GetPosition().z; iz < 3; iz++)
            {
                for (int ix = camera.GetPosition().x; ix < 3; ix++)
                    m_Data.insert({{ ix, 0, iz }, Chunk({ ix * 16, 0, iz * 16 })});
            }
        }

        // Some test functions
        Chunk* GetChunk(const glm::vec3& position)
        {
            return &m_Data.at(GlobalToChunkPosition(position));
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
                auto* ch = w.GetChunk(camera.GetPosition());
                auto breakPos = GlobalVoxelToLocal(GlobalToVoxel(camera.GetPosition()));
                breakPos.y = 127;
                ch->m_ChunkData.At(breakPos).type = BlockType::Air;
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