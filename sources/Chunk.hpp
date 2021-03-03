#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "ChunkData.hpp"
#include "Vertex.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class Chunk
{
    private:
        std::shared_ptr<VertexArray>            m_VA;
        std::shared_ptr<VertexBuffer>           m_VB;
        std::shared_ptr<IndexBuffer>            m_IB;

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

        bool BlockInBounds(const glm::ivec3& pos)
        {
            return !(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= 16 || pos.y >= 256 || pos.z >= 16);
        }

        void Raycast(glm::vec3 origin, glm::vec3 dir, float distance)
        {
            float t = 0.0f;

            glm::ivec3 i = glm::ivec3(
                static_cast<int32_t>(floor(origin.x)),
                static_cast<int32_t>(floor(origin.y)),
                static_cast<int32_t>(floor(origin.z))
            );

            glm::ivec3 step = glm::ivec3(
                (dir.x > 0) ? 1 : -1,
                (dir.y > 0) ? 1 : -1,
                (dir.z > 0) ? 1 : -1
            );

            glm::vec3 tDelta = glm::vec3(abs(1 / dir.x), abs(1 / dir.y), abs(1 / dir.z));
            glm::vec3 dist = glm::vec3(
                (step.x > 0) ? (i.x + 1 - origin.x) : (origin.x - i.x),
                (step.y > 0) ? (i.y + 1 - origin.y) : (origin.y - i.y),
                (step.z > 0) ? (i.z + 1 - origin.z) : (origin.z - i.z)
            );

            glm::vec3 max = tDelta * dist;
            int32_t steppedIndex = -1;

            while (t <= distance) 
            {
                if (BlockInBounds({ i.x, i.y, i.z }) && m_ChunkData.At({ i.x, i.y, i.z }).type != BlockType::Air)
                {
                    m_ChunkData.At({ i.x, i.y, i.z }).type = BlockType::Air;
                    m_Vertices.clear();
                    m_Indices.clear();
                    GenerateMesh();
                    return;
                }

                if (max.x < max.y) 
                {
                    if (max.x < max.z) 
                    {
                        i.x += step.x;
                        t = max.x;
                        max.x += tDelta.x;
                        steppedIndex = 0;
                    } 
                    else 
                    {
                        i.z += step.z;
                        t = max.z;
                        max.z += tDelta.z;
                        steppedIndex = 2;
                    }
                } 
                else 
                {
                    if (max.y < max.z) 
                    {
                        i.y += step.y;
                        t = max.y;
                        max.y += tDelta.y;
                        steppedIndex = 1;
                    } 
                    else 
                    {
                        i.z += step.z;
                        t = max.z;
                        max.z += tDelta.z;
                        steppedIndex = 2;
                    }
                }
            }
        }
        void Draw();
};