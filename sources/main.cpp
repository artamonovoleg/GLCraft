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

class Camera
{
    private:
        glm::vec3 m_Position;
        glm::vec3 m_ViewDirection = glm::vec3(0, 0, -1);
        glm::vec3 m_Up = glm::vec3(0, 1, 0);

        glm::mat4 m_Projection;
        glm::mat4 m_View;

        const float m_Speed = 0.05f;

        const Keyboard& keyboard = Engine::GetEventSystem()->GetKeyboard();

        void Recalculate()
        {
            m_View = glm::lookAt(m_Position, m_Position + m_ViewDirection, m_Up);
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
            if (keyboard.GetKey(GLFW_KEY_W))
                m_Position += m_Speed * m_ViewDirection;
            if (keyboard.GetKey(GLFW_KEY_S))
                m_Position -= m_Speed * m_ViewDirection;
            if (keyboard.GetKey(GLFW_KEY_A))
                m_Position -= glm::normalize(glm::cross(m_ViewDirection, m_Up)) * m_Speed;
            if (keyboard.GetKey(GLFW_KEY_D))
                m_Position += glm::normalize(glm::cross(m_ViewDirection, m_Up)) * m_Speed;
            
            Recalculate();
        }

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetViewDirection() const { return m_ViewDirection; }

        const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        const glm::mat4& GetViewMatrix() const { return m_View; }
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
                            
        Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
        Texture mainTexture("../assets/textures/atlas.png");

        Chunk chunk ( { -3, 0, -5 } );

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
            
            chunk.Draw();

            // w.Draw();
            auto castres = chunk.RayCast(camera.GetPosition(), camera.GetViewDirection(), 5.0f);
            if (castres.has_value())
            {
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    castres->chunk->Set(castres->blockPosition, BlockType::Air);
                if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
                    castres->chunk->Set(castres->blockPosition + castres->normal, BlockType::Grass);
            }
                
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}