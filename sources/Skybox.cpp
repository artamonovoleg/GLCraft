#include "Skybox.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "TextureCubemap.hpp"
#include "Shader.hpp"

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

Skybox::Skybox(const std::string& folder, const std::string& ext)
{
    std::vector<std::string> faces { 
                                        folder + "right." + ext,
                                        folder + "left." + ext,
                                        folder + "top." + ext,
                                        folder + "bottom." + ext,
                                        folder + "front." + ext,
                                        folder + "back." + ext
                                    };
    m_VA = std::make_shared<VertexArray>();
    m_VB = std::make_shared<VertexBuffer>();
    m_VB->Map(skyboxVertices, sizeof(skyboxVertices));
    VertexBuffersLayout layout;
    layout.Push<float>(3, 3);

    m_Shader = std::make_shared<Shader>("../shaders/sbvert.glsl", "../shaders/sbfrag.glsl");
    m_Texture = std::make_shared<TextureCubemap>(faces);

    m_Shader->Bind();
    m_Shader->SetInt("skybox", 0);
}

void Skybox::Draw(const glm::mat4& proj, const glm::mat4& view)
{
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_Shader->Bind();
    m_Shader->SetMat4("view", glm::mat4(glm::mat3(view)));
    m_Shader->SetMat4("projection", proj);
    // skybox cube
    m_VA->Bind();
    m_Texture->Bind();
    glDrawArrays(GL_TRIANGLES, 0, sizeof(skyboxVertices) / sizeof(float));
    glDepthFunc(GL_LESS); // set depth function back to default
}