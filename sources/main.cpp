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
#include "Camera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

using voxelID = uint8_t;

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 tex;
};

namespace VoxelID
{
    enum : voxelID
    {
        Air     = 0,
        Grass   = 1,
        Ground  = 2,
        Sand    = 3,
        Glass   = 4
    };
};

class SpriteLibrary
{
    private:
        float m_SpritesInSide;
        std::unordered_map<voxelID, glm::ivec2> m_Sprites;

        /// return left up sprite corner
        glm::vec2 GetSprite(const glm::ivec2& pos)
        {
            return { pos.x / m_SpritesInSide, pos.y / m_SpritesInSide };
        }
    public:
        SpriteLibrary()
            : m_SpritesInSide(16.0f)
        {
            m_Sprites[VoxelID::Grass]   = { 0, 0 };
            m_Sprites[VoxelID::Ground]  = { 1, 0 };
            m_Sprites[VoxelID::Sand]    = { 2, 0 };
            m_Sprites[VoxelID::Glass]   = { 3, 0 };
        }

        glm::vec2 GetSprite(voxelID id)
        {
            return GetSprite(m_Sprites.at(id));
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