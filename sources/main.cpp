#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Chunk.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Block* RayCast(Chunk& chunk, glm::vec3& a, glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) 
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
 
    while (t <= maxDist){
        Block* block = chunk.GetBlock( { ix, iy, iz });
        if (block != nullptr)
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
            return block;
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

int main()
{
    Engine::Init({ 1920, 1080, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 8.0f, 128.0f, 8.0f });
        
        Skybox skybox({     "../assets/skybox/right.jpg",
                            "../assets/skybox/left.jpg",
                            "../assets/skybox/top.jpg",
                            "../assets/skybox/bottom.jpg",
                            "../assets/skybox/front.jpg",
                            "../assets/skybox/back.jpg" });
                            
        Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
        Texture mainTexture("../assets/textures/atlas.png");

        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;

        Chunk chunk ( { 0, 0, 0 } );

        va.Bind();

        vb.Bind();
        vb.Map(chunk.GetVertices().data(), chunk.GetVerticesSize());
        vb.SetLayout();

        ib.Bind();
        ib.Map(chunk.GetIndices().data(), chunk.GetIndicesSize());

        va.Unbind();

        mainTexture.Bind();
        mainShader.Bind();
        mainShader.SetInt("texture1", 0);

        mainShader.SetMat4("u_Projection", proj);

        glEnable(GL_DEPTH_TEST);

        glm::vec3 a;
        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);

            glClearColor(0.2, 0.3, 0.4, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
            mainTexture.Bind();
            mainShader.Bind();
            mainShader.SetMat4("u_View", camera.GetViewMatrix());
            
            va.Bind();

            glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);

            if (keyboard.GetKeyDown(GLFW_KEY_C))
            {
                Block* b = RayCast(chunk, camera.Position, camera.Front, 10.0f, a, a, a);
                if (b != nullptr)
                    std::cout << b->type << std::endl;
            }
            skybox.Draw(proj, camera.GetViewMatrix());
            Engine::GetWindow()->SwapBuffers();
            Engine::GetEventSystem()->Process();
        }
    }
    Engine::Shutdown();
}