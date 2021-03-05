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

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 WorldToChunk(const glm::vec3& coords)
{
    return { coords.x / 16, coords.y, coords.z / 16 };
}

class Camera
{
    private:
        glm::vec3 m_Position;
        glm::vec3 m_ViewDirection = glm::vec3(0, 0, -1);
        glm::vec3 m_Up = glm::vec3(0, 1, 0);
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp = glm::vec3(0, 1, 0);

        glm::mat4 m_Projection;
        glm::mat4 m_View;

        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;

        const float m_Speed = 4.5f;

        void RecalculateViewMatrix()
        {
            m_View = glm::lookAt(m_Position, m_Position + m_ViewDirection, m_Up);
        }

        void RecalculateVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            front.y = sin(glm::radians(m_Pitch));
            front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            m_ViewDirection = glm::normalize(front);
            // also re-calculate the Right and Up vector
            m_Right = glm::normalize(glm::cross(m_ViewDirection, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            m_Up    = glm::normalize(glm::cross(m_Right, m_ViewDirection));
        }
    public:
        Camera(const glm::vec3& position)
            : m_Position(position)
        {
            m_Projection = glm::perspective(glm::radians(45.0f), Engine::GetWindow()->GetWidth() / static_cast<float>(Engine::GetWindow()->GetHeight()), 0.1f, 30.0f);
            m_View = glm::lookAt(m_Position, m_Position - m_ViewDirection, m_Up);
        }
        
        void Update(float dt)
        {
            static auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
            static auto& mouse = Engine::GetEventSystem()->GetMouse();

            if (keyboard.GetKey(GLFW_KEY_W))
                m_Position += m_Speed * m_ViewDirection * dt;
            if (keyboard.GetKey(GLFW_KEY_S))
                m_Position -= m_Speed * m_ViewDirection * dt;
            if (keyboard.GetKey(GLFW_KEY_A))
                m_Position -= glm::normalize(glm::cross(m_ViewDirection, m_Up)) * m_Speed * dt;
            if (keyboard.GetKey(GLFW_KEY_D))
                m_Position += glm::normalize(glm::cross(m_ViewDirection, m_Up)) * m_Speed * dt;

            float xoffset = mouse.GetOffsetX();
            float yoffset = mouse.GetOffsetY();

            if (xoffset != 0 || yoffset != 0)
            {
                xoffset *= 0.1;
                yoffset *= 0.1;

                m_Yaw   += xoffset;
                m_Pitch += yoffset;

                if (m_Pitch > 89.0f)
                    m_Pitch = 89.0f;
                if (m_Pitch < -89.0f)
                    m_Pitch = -89.0f;

                RecalculateVectors();
            }

            RecalculateViewMatrix();
        }

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetViewDirection() const { return m_ViewDirection; }

        const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }
};

class World
{
    private:
        const Camera& m_Camera;
        std::unordered_map<glm::ivec2, Chunk> m_Data;
    public:
        World(const Camera& camera)
            : m_Camera(camera)
        {
            for (int iz = camera.GetPosition().z / 16; iz < 3; iz++)
            {
                for (int ix = camera.GetPosition().x / 16; ix < 3; ix++)
                    m_Data.insert({{ ix, iz }, Chunk({ ix * 16, 0, iz * 16 })});
            }
        }

        void Draw()
        {
            static auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
            static auto& mouse = Engine::GetEventSystem()->GetMouse();
            auto castres = m_Data.at({ m_Camera.GetPosition().x / 16, m_Camera.GetPosition().z / 16 }).RayCast(m_Camera.GetPosition(), m_Camera.GetViewDirection(), 5.0f);
            if (castres.has_value())
            {
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    castres->chunk->Set(castres->blockPosition, BlockType::Air);
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                    castres->chunk->Set(castres->blockPosition + castres->normal, BlockType::Grass);
            }
            for (auto& p : m_Data)
            {
                p.second.Draw();
            }
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

        Camera camera({ -16, 129, -16 });
        
        Skybox skybox({     "../assets/skybox/right.jpg",
                            "../assets/skybox/left.jpg",
                            "../assets/skybox/top.jpg",
                            "../assets/skybox/bottom.jpg",
                            "../assets/skybox/front.jpg",
                            "../assets/skybox/back.jpg" });
                            
        Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
        Texture mainTexture("../assets/textures/atlas.png");

        // Chunk chunk ( { -16, 0, -16 } );

        World w (camera);

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
            
            // chunk.Draw();

            w.Draw();
            // auto castres = chunk.RayCast(camera.GetPosition(), camera.GetViewDirection(), 5.0f);
            // if (castres.has_value())
            // {
            //     if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            //         castres->chunk->Set(castres->blockPosition, BlockType::Air);
            //     if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
            //         castres->chunk->Set(castres->blockPosition + castres->normal, BlockType::Grass);
            // }
                
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}