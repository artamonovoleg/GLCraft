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
#include "WorldTransform.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

class World
{
    private:
        std::shared_ptr<Shader>     m_Shader;
        std::shared_ptr<Texture>    m_Texture;

        const Camera& m_Camera;
        std::unordered_map<glm::ivec2, Chunk> m_Data;
    public:
        World(const Camera& camera)
            : m_Camera(camera)
        {
            m_Shader = std::make_shared<Shader>("../shaders/vert.glsl", "../shaders/frag.glsl");
            m_Texture = std::make_shared<Texture>("../assets/textures/atlas.png");
            m_Shader->SetInt("texture1", 0);

            for (int iz = camera.GetPosition().z / 16; iz < 3; iz++)
            {
                for (int ix = camera.GetPosition().x / 16; ix < 3; ix++)
                    m_Data.insert({{ ix, iz }, Chunk({ ix * 16, 0, iz * 16 })});
            }
        }

        void Draw()
        {
            m_Texture->Bind();
            m_Shader->Bind();
            m_Shader->SetMat4("u_Model", glm::mat4(1.0));
            m_Shader->SetMat4("u_View", m_Camera.GetViewMatrix());
            m_Shader->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());

            static auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
            static auto& mouse = Engine::GetEventSystem()->GetMouse();
            auto castres = m_Data.at(glm::ivec2(m_Camera.GetPosition().x, m_Camera.GetPosition().z) / 16).RayCast(m_Camera.GetPosition(), m_Camera.GetViewDirection(), 5.0f);
            if (castres.has_value())
            {
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    castres->chunk->Set(castres->blockPosition, BlockType::Air);
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                    castres->chunk->Set(castres->blockPosition + castres->normal, BlockType::Grass);
                std::cout << castres->blockPosition.x << " " << castres->blockPosition.y << " " << castres->blockPosition.z << std::endl;
            }
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
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            Engine::GetWindow()->SwapBuffers();
        }
    }
    Engine::Shutdown();
}