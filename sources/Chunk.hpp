#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "ChunkData.hpp"
#include "Vertex.hpp"

static std::ostream& operator << (std::ostream& os, const BlockType& type)
{
    switch (type)
    {
        case BlockType::Air:
            os << "Air";
            break;
        case BlockType::Grass:
            os << "Grass";
            break;
    }
    return os;
}

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
        Chunk(glm::ivec3 pos);

        void PrintBlockType(const glm::vec3& pos) 
        {
            auto l = pos - m_Pos;
            if (l.x >= 0 && l.x < 16 && l.y >= 0 && l.y < 256 && l.z >= 0 && l.z < 256)
                std::cout << m_ChunkData.At(pos - m_Pos).type << std::endl;
            else
                std::cout << "Air" << std::endl;
        }

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        size_t GetVerticesSize() const { return m_Vertices.size() * sizeof(Vertex); }
        std::vector<unsigned int>& GetIndices() { return m_Indices; }
        size_t GetIndicesSize() const { return m_Indices.size() * sizeof(unsigned int); }
};