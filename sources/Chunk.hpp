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

        Block* GetBlock(const glm::ivec3& pos)
        {
            if (BlockInBounds(pos))
                return &m_ChunkData.At(pos);
            else
                return nullptr;
        }

        void RayCast(const glm::vec3& startPoint, const glm::vec3& direction, float range)
        {
            // Ensures passed direction is normalized
            auto nDirection = glm::normalize(direction);
            auto endPoint = startPoint + nDirection * range;
            glm::ivec3 startVoxel;
            {
                auto x = static_cast<int>(std::floor(startPoint.x));
                auto y = static_cast<int>(std::floor(startPoint.y));
                auto z = static_cast<int>(std::floor(startPoint.z));
                startVoxel = { x, y, z};
            }
            // +1, -1, or 0
            int stepX = (nDirection.x > 0) ? 1 : ((nDirection.x < 0) ? -1 : 0);
            int stepY = (nDirection.y > 0) ? 1 : ((nDirection.y < 0) ? -1 : 0);
            int stepZ = (nDirection.z > 0) ? 1 : ((nDirection.z < 0) ? -1 : 0);

            float tDeltaX =
                (stepX != 0) ? fmin(stepX / (endPoint.x - startPoint.x), FLT_MAX) : FLT_MAX;
            float tDeltaY =
                (stepY != 0) ? fmin(stepY / (endPoint.y - startPoint.y), FLT_MAX) : FLT_MAX;
            float tDeltaZ =
                (stepZ != 0) ? fmin(stepZ / (endPoint.z - startPoint.z), FLT_MAX) : FLT_MAX;

            float tMaxX = (stepX > 0.0f) ? tDeltaX * (1.0f - startPoint.x + startVoxel.x)
                                        : tDeltaX * (startPoint.x - startVoxel.x);
            float tMaxY = (stepY > 0.0f) ? tDeltaY * (1.0f - startPoint.y + startVoxel.y)
                                        : tDeltaY * (startPoint.y - startVoxel.y);
            float tMaxZ = (stepZ > 0.0f) ? tDeltaZ * (1.0f - startPoint.z + startVoxel.z)
                                        : tDeltaZ * (startPoint.z - startVoxel.z);

            auto currentVoxel = startVoxel;

            int r = 0;
            // sanity check to prevent leak
            while (r < range * 3) 
            {
                Block* block = GetBlock({ currentVoxel.x, currentVoxel.y, currentVoxel.z });
                if (block != nullptr && block->type != BlockType::Air)
                {
                    m_ChunkData.At(currentVoxel).type = BlockType::Air;
                    m_Vertices.clear();
                    m_Indices.clear();
                    GenerateMesh();
                    return;
                }
                if (tMaxX < tMaxY) 
                {
                    if (tMaxX < tMaxZ) 
                    {
                        currentVoxel.x += stepX;
                        tMaxX += tDeltaX;
                    }
                    else 
                    {
                        currentVoxel.z += stepZ;
                        tMaxZ += tDeltaZ;
                    }
                }
                else 
                {
                    if (tMaxY < tMaxZ) 
                    {
                        currentVoxel.y += stepY;
                        tMaxY += tDeltaY;
                    }
                    else 
                    {
                        currentVoxel.z += stepZ;
                        tMaxZ += tDeltaZ;
                    }
                }
                if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
                    break;
                r++;
            }
            return;
        }

        void Draw();
};