#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "ChunkData.hpp"
#include "Vertex.hpp"
#include "Coordinates.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class Chunk;

struct RaycastResult
{
    glm::ivec3 blockPosition;
    glm::ivec3 normal;
    Chunk*     chunk;
};

class World;

class Chunk
{
    public:
        std::shared_ptr<VertexArray>            m_VA;
        std::shared_ptr<VertexBuffer>           m_VB;
        std::shared_ptr<IndexBuffer>            m_IB;

        glm::ivec3                  m_Pos;
        ChunkData<16, 256, 16>      m_ChunkData;
        std::vector<Vertex>         m_Vertices;
        std::vector<unsigned int>   m_Indices;

        // Push face into given pos. (In chunk coordinate system not world global)
        void PushLeft   (const glm::ivec3& pos);
        void PushRight  (const glm::ivec3& pos);
        void PushBottom (const glm::ivec3& pos);
        void PushTop    (const glm::ivec3& pos);
        void PushBack   (const glm::ivec3& pos);
        void PushFront  (const glm::ivec3& pos);

        /// Add 6 more indices
        void PushIndices();

        /// Generate landscape and save map setup to chunk data
        void GenerateLandscape();
        /// Push all vertices and indices
        void GenerateMesh();
    public:
        Chunk(glm::ivec3 pos);
        void Draw();
};