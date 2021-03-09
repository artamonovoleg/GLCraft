#pragma once

#include <memory>
#include <vector>
#include "Vertex.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;

struct Mesh
{
    std::shared_ptr<VertexArray>    va;
    std::shared_ptr<VertexBuffer>   vb;
    std::shared_ptr<IndexBuffer>    ib;

    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;

    Mesh();
    void Load();
    void Draw();
};