#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "ChunkData.hpp"
#include "Vertex.hpp"
#include "WorldTransform.hpp"

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

        Block* GetBlock(const glm::ivec3& pos)
        {
            if (BlockInBounds(pos))
                return &m_ChunkData.At(pos);
            else
                return nullptr;
        }
        
    public:
        Chunk(glm::ivec3 pos);

        bool BlockInBounds(const glm::ivec3& pos)
        {
            return !(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= 16 || pos.y >= 256 || pos.z >= 16);
        }

        std::optional<RaycastResult> RayCast(const glm::vec3& startP, const glm::vec3& direction, float range)
        {
            glm::vec3 startPoint { startP.x - m_Pos.x, startP.y, startP.z - m_Pos.z };
            auto nDirection = glm::normalize(direction);
            auto endPoint = startPoint + nDirection * range;

            glm::ivec3 norm;
            glm::ivec3 iend;

            glm::ivec3 startVoxel /* = GlobalToVoxel(startPoint); */ = ToLocalVoxelPosition(GlobalToVoxel(startP));

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

            float r = 0.0f;
            int steppedIndex = -1;

            // sanity check to prevent leak
            while (r < range * 3) 
            {
                Block* block = GetBlock({ currentVoxel.x, currentVoxel.y, currentVoxel.z });
                if (block != nullptr && block->type != BlockType::Air)
                {
                    iend.x = std::floor(startPoint.x);
                    iend.y = std::floor(startPoint.y);
                    iend.z = std::floor(startPoint.z);
        
                    norm.x = norm.y = norm.z = 0;
                    if (steppedIndex == 0) norm.x = -stepX;
                    if (steppedIndex == 1) norm.y = -stepY;
                    if (steppedIndex == 2) norm.z = -stepZ;

                    return RaycastResult{ currentVoxel, norm, this };
                }
                if (tMaxX < tMaxY) 
                {
                    if (tMaxX < tMaxZ) 
                    {
                        currentVoxel.x += stepX;
                        tMaxX += tDeltaX;
                        steppedIndex = 0;
                    }
                    else 
                    {
                        currentVoxel.z += stepZ;
                        tMaxZ += tDeltaZ;
                        steppedIndex = 2;
                    }
                }
                else 
                {
                    if (tMaxY < tMaxZ) 
                    {
                        currentVoxel.y += stepY;
                        tMaxY += tDeltaY;
                        steppedIndex = 1;
                    }
                    else 
                    {
                        currentVoxel.z += stepZ;
                        tMaxZ += tDeltaZ;
                        steppedIndex = 2;
                    }
                }
                if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
                    break;
                r++;
            }

            return {};
        }

        void Set(const glm::ivec3& pos, BlockType type)
        {
            m_ChunkData.At(pos).type = type;
            m_Vertices.clear();
            m_Indices.clear();
            GenerateMesh();
        }

        void Draw();
};