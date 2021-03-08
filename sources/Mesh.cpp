#include "Mesh.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffersLayout.hpp"

void Mesh::Upload()
{
    va = std::make_shared<VertexArray>  ();
    vb = std::make_shared<VertexBuffer> ();
    ib = std::make_shared<IndexBuffer>  ();

    vb->Map(vertices.data(), vertices.size() * sizeof(Vertex));
    VertexBuffersLayout la;
    la.Push<float>(3, 5);
    la.Push<float>(2, 5);
    ib->Map(indices.data(), indices.size() * sizeof(unsigned int));
}