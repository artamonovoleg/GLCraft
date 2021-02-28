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

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    Engine::Init({ 1920, 1080, "Minecraft" });
    Camera camera({ 0.0f, 0.0f, 3.0f });
    
    Shader mainShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    Texture mainTexture("../assets/textures/atlas.png");
    Shader skyboxShader("../shaders/sbvert.glsl", "../shaders/sbfrag.glsl");
    TextureCubemap skyboxTex({  "../assets/skybox/right.jpg",
                                "../assets/skybox/left.jpg",
                                "../assets/skybox/top.jpg",
                                "../assets/skybox/bottom.jpg",
                                "../assets/skybox/front.jpg",
                                "../assets/skybox/back.jpg" });

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;

    Chunk chunk ({ 0, 0, 0 });

    va.Bind();

    vb.Bind();
    vb.Map(chunk.GetVertices().data(), chunk.GetVerticesSize());
    vb.SetLayout();

    ib.Bind();
    ib.Map(chunk.GetIndices().data(), chunk.GetIndicesSize());

    va.Unbind();

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
    auto& mouse = Engine::GetEventSystem()->GetMouse();

    mainTexture.Bind();
    mainShader.Bind();
    mainShader.SetInt("texture1", 0);

    auto& window = Engine::GetWindow();
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

    mainShader.SetMat4("u_Projection", proj);

    skyboxShader.Bind();
    skyboxShader.SetInt("skybox", 0);
    glEnable(GL_DEPTH_TEST);

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

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Bind();
        skyboxShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        skyboxShader.SetMat4("projection", proj);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        skyboxTex.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        Engine::GetWindow()->SwapBuffers();
        Engine::GetEventSystem()->Process();
    }
}