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

        void PrintBlockType(const glm::ivec3& pos) 
        {
            auto l = pos;
            if (l.x >= 0 && l.x < 16 && l.y >= 0 && l.y < 256 && l.z >= 0 && l.z < 256)
                std::cout << m_ChunkData.At(pos).type << std::endl;
            else
                std::cout << "Air" << std::endl;
        }

        struct RaycastResult {
            glm::vec3 position;
            glm::ivec3 blockPosition;
            glm::ivec3 normal;
            Chunk* chunk;
        };

        RaycastResult Raycast(glm::vec3 origin, glm::vec3 dir, float distance) 
        {
            float t = 0.0f;

            glm::ivec3 i = glm::ivec3(
                static_cast<int32_t>(floor(origin.x)),
                static_cast<int32_t>(floor(origin.y)),
                static_cast<int32_t>(floor(origin.z))
            );

            glm::ivec3 chunkPos = m_Pos;
            glm::ivec3 pos = origin;

            glm::ivec3 step = glm::ivec3(
                (dir.x > 0) ? 1 : -1,
                (dir.y > 0) ? 1 : -1,
                (dir.z > 0) ? 1 : -1
            );

            glm::vec3 tDelta = glm::vec3(std::abs(1 / dir.x), std::abs(1 / dir.y), std::abs(1 / dir.z));
            glm::vec3 dist = glm::vec3(
                (step.x > 0) ? (i.x + 1 - origin.x) : (origin.x - i.x),
                (step.y > 0) ? (i.y + 1 - origin.y) : (origin.y - i.y),
                (step.z > 0) ? (i.z + 1 - origin.z) : (origin.z - i.z)
            );

            glm::vec3 max = tDelta * dist;
            int32_t steppedIndex = 0;

            Chunk* currentChunk = this;

            while (t <= distance) {

                if (currentChunk != nullptr) 
                {
                    Block& block = m_ChunkData.At(pos);
                    BlockType& type = m_ChunkData.At(pos).type;

                        RaycastResult result;
                        result.blockPosition = i;
                        result.position = origin + t * dir;
                        result.normal = {};
                        result.normal[steppedIndex] = -step[steppedIndex];
                        result.chunk = currentChunk;

                        return result;
                }

                if (max.x < max.y) {
                    if (max.x < max.z) {
                        i.x += step.x;
                        pos.x += step.x;
                        t = max.x;
                        max.x += tDelta.x;
                        steppedIndex = 0;
                    } else {
                        i.z += step.z;
                        pos.z += step.z;
                        t = max.z;
                        max.z += tDelta.z;
                        steppedIndex = 2;
                    }
                } else {
                    if (max.y < max.z) {
                        i.y += step.y;
                        pos.y += step.y;
                        t = max.y;
                        max.y += tDelta.y;
                        steppedIndex = 1;
                    } else {
                        i.z += step.z;
                        pos.z += step.z;
                        t = max.z;
                        max.z += tDelta.z;
                        steppedIndex = 2;
                    }
                }
            }

            return {};
        }

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        size_t GetVerticesSize() const { return m_Vertices.size() * sizeof(Vertex); }
        std::vector<unsigned int>& GetIndices() { return m_Indices; }
        size_t GetIndicesSize() const { return m_Indices.size() * sizeof(unsigned int); }
};