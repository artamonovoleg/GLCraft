#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "ChunkData.hpp"
#include "Vertex.hpp"

class Chunk
{
    private:
        glm::vec3                   m_Pos;
        ChunkData<16, 256, 16>      m_ChunkData;
        std::vector<Vertex>         m_Vertices;
        std::vector<unsigned int>   m_Indices;

        // Push face into given pos. (In chunk coordinate system not world global)
        void PushLeft(const glm::vec3& pos);
        void PushRight(const glm::vec3& pos);
        void PushBottom(const glm::vec3& pos);
        void PushTop(const glm::vec3& pos);
        void PushBack(const glm::vec3& pos);
        void PushFront(const glm::vec3& pos);

        /// Add 6 more indices
        void PushIndices();

        /// Generate landscape and save map setup to chunk data
        void GenerateLandscape();
        /// Push all vertices and indices
        void GenerateMesh();
    public:
        Chunk(const glm::ivec3& pos);

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        size_t GetVerticesSize() const { return m_Vertices.size() * sizeof(Vertex); }
        std::vector<unsigned int>& GetIndices() { return m_Indices; }
        size_t GetIndicesSize() const { return m_Indices.size() * sizeof(unsigned int); }
};