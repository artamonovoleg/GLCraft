#include "Mesh.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"

Mesh::Mesh()
{
    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer>();
    VertexBuffersLayout layout;
    layout.Push<float>(3, 6);
    layout.Push<float>(2, 6);
    layout.Push<float>(1, 6);
    ib = std::make_shared<IndexBuffer>();
}

void Mesh::Load()
{
    va->Bind();
    vb->Map(vertices.data(), vertices.size() * sizeof(vertices.at(0)));
    ib->Map(indices.data(), indices.size() * sizeof(indices.at(0)));
}

void Mesh::Draw() const
{
    va->Bind();
    glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}