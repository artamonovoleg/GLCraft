#pragma once

#include <iostream>
#include <vector>
#include <memory>
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

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class Chunk
{
    private:
        std::shared_ptr<VertexArray>    m_VA;
        std::shared_ptr<VertexBuffer>   m_VB;
        std::shared_ptr<IndexBuffer>    m_IB;

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
            if (pos.x < 0 || pos.x >= 16) return false;
            if (pos.y < 0 || pos.y >= 128) return false;
            if (pos.z < 0 || pos.z >= 16) return false;
            return true;
        } 

        Block GetBlock(const glm::ivec3& pos) 
        {
            if (BlockInBounds(pos))
                return m_ChunkData.At(pos);
            else
                return {};
        }

        void RayCast(glm::vec3& a, glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) 
        {
            float px = a.x;
            float py = a.y;
            float pz = a.z;
        
            float dx = dir.x;
            float dy = dir.y;
            float dz = dir.z;
        
            float t = 0.0f;
            int ix = floor(px);
            int iy = floor(py);
            int iz = floor(pz);
        
            float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
            float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
            float stepz = (dz > 0.0f) ? 1.0f : -1.0f;
        
            float infinity = std::numeric_limits<float>::infinity();
        
            float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
            float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
            float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);
        
            float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
            float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
            float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);
        
            float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
            float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
            float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;
        
            int steppedIndex = -1;
        
            while (t <= maxDist)
            {
                Block block = GetBlock( { ix, iy, iz } );
                if (block.type != BlockType::Air)
                {
                    end.x = px + t * dx;
                    end.y = py + t * dy;
                    end.z = pz + t * dz;
        
                    iend.x = ix;
                    iend.y = iy;
                    iend.z = iz;
        
                    norm.x = norm.y = norm.z = 0.0f;
                    if (steppedIndex == 0) norm.x = -stepx;
                    if (steppedIndex == 1) norm.y = -stepy;
                    if (steppedIndex == 2) norm.z = -stepz;
                    m_ChunkData.At({ ix, iy + 1, iz }).type = BlockType::Air;
                    GenerateMesh();
                    return;
                }

                if (txMax < tyMax) 
                {
                    if (txMax < tzMax) 
                    {
                        ix += stepx;
                        t = txMax;
                        txMax += txDelta;
                        steppedIndex = 0;
                    } 
                    else 
                    {
                        iz += stepz;
                        t = tzMax;
                        tzMax += tzDelta;
                        steppedIndex = 2;
                    }
                } 
                else 
                {
                    if (tyMax < tzMax) 
                    {
                        iy += stepy;
                        t = tyMax;
                        tyMax += tyDelta;
                        steppedIndex = 1;
                    } 
                    else 
                    {
                        iz += stepz;
                        t = tzMax;
                        tzMax += tzDelta;
                        steppedIndex = 2;
                    }
                }
            }
            iend.x = ix;
            iend.y = iy;
            iend.z = iz;
        
            end.x = px + t * dx;
            end.y = py + t * dy;
            end.z = pz + t * dz;
            norm.x = norm.y = norm.z = 0.0f;
        }

        void Draw();
};