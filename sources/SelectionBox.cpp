#include <glm/gtc/matrix_transform.hpp>
#include "SelectionBox.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "Camera.hpp"

namespace
{
    float vertices[] = {
        // positions          
        -1.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        0.0f, -1.0f, -1.0f,
        0.0f, -1.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        -1.0f,  0.0f, -1.0f,

        -1.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  0.0f, -1.0f,
        -1.0f,  0.0f, -1.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,

        0.0f, -1.0f, -1.0f,
        0.0f, -1.0f,  0.0f,
        0.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  0.0f,
        0.0f,  0.0f, -1.0f,
        0.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 0.0f,
        -1.0f,  0.0f, 0.0f,
        0.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,

        -1.0f,  0.0f, -1.0f,
        0.0f,   0.0f, -1.0f,
        0.0f,   0.0f,  0.0f,
        0.0f,   0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  0.0f,
        0.0f, -1.0f, -1.0f,
        0.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f,  0.0f
    };
};

SelectionBox::SelectionBox()
{
    m_VA = std::make_shared<VertexArray>();
    m_VB = std::make_shared<VertexBuffer>();
    m_VB->Map(vertices, sizeof(vertices));
    VertexBuffersLayout layout;
    layout.Push<float>(3, 3);

    m_Shader = std::make_shared<Shader>("../shaders/selectionbox.vert", "../shaders/selectionbox.frag");
}

void SelectionBox::Draw(const Camera& camera, const RaycastResult& result)
{
    m_Shader->Bind();
    float size = 1.00001f;
    m_Shader->SetMat4("u_Model", glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(size, size, size)), { result.end.x + 1, result.end.y + 1, result.end.z + 1 }) );
    m_Shader->SetMat4("u_View", camera.GetViewMatrix());
    m_Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
    m_VA->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}