#include "Crosshair.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"

float vertices [] = 
{   -0.03, 0.0, 
    0.03, 0.0, 
    
    0.0, 0.05, 
    0.0, -0.05 
};

Crosshair::Crosshair()
{
    m_VA = std::make_shared<VertexArray>();
    m_VB = std::make_shared<VertexBuffer>();
    m_VB->Map(vertices, sizeof(vertices));
    VertexBuffersLayout layout;
    layout.Push<float>(2, 2);

    m_Shader = std::make_shared<Shader>("../shaders/crosshair.vert", "../shaders/crosshair.frag");
}

void Crosshair::Draw()
{
    m_Shader->Bind();
    m_VA->Bind();
    glDrawArrays(GL_LINES, 0, 8);
}