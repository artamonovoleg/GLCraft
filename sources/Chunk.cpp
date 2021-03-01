#include "Chunk.hpp"
#include "Block.hpp"

#include <iostream>

Chunk::Chunk(glm::ivec3 pos)
    : m_Pos(std::move(pos))
{
    GenerateLandscape();
	GenerateMesh();
}

void Chunk::PushLeft(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, -0.5)   + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, 0.5)    + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 0.0f) });

    PushIndices();
}

void Chunk::PushRight(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, 0.5)    + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, -0.5)   + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 0.0f) });

    PushIndices();
}

void Chunk::PushBottom(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(0.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(0.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, -0.5)   + m_Pos + pos, .tex = glm::vec2(1.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, 0.5)    + m_Pos + pos, .tex = glm::vec2(1.0f / 16.0f, 1.0f / 16.0f) });

    PushIndices();
}

void Chunk::PushTop(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(0.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(0.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, -0.5)     + m_Pos + pos, .tex = glm::vec2(1.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, 0.5)      + m_Pos + pos, .tex = glm::vec2(1.0f / 16.0f, 1.0f / 16.0f) });

    PushIndices();
}

void Chunk::PushBack(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, -0.5)   + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, -0.5)     + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, -0.5)    + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 0.0f) });

    PushIndices();
}

void Chunk::PushFront(const glm::vec3& pos)
{
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, -0.5, 0.5)    + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, -0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 1.0f / 16.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(-0.5, 0.5, 0.5)     + m_Pos + pos, .tex = glm::vec2(3.0f / 16.0f, 0.0f) });
    m_Vertices.push_back({ .pos = glm::vec3(0.5, 0.5, 0.5)      + m_Pos + pos, .tex = glm::vec2(4.0f / 16.0f, 0.0f) });

    PushIndices();
}

void Chunk::PushIndices()
{
    uint32_t m;
    if (!m_Indices.empty())
        m = m_Indices.back() + 1;
    else
        m = 0;
    m_Indices.insert(m_Indices.end(), { 0 + m, 1 + m, 2 + m, 2 + m, 1 + m, 3 + m });
}

void Chunk::GenerateLandscape()
{
    for (size_t z = 0; z < m_ChunkData.Z(); z++)
    {
        for (size_t y = 0; y < m_ChunkData.Y(); y++)
        {
            for (size_t x = 0; x < m_ChunkData.X(); x++)
                m_ChunkData.At({x, y, z}).type = BlockType::Grass;
        }
    }
}

void Chunk::GenerateMesh()
{
    for (int z = 0; z < m_ChunkData.Z(); z++)
    {
        for (int y = 0; y < m_ChunkData.Y(); y++)
        {
            for (int x = 0; x < m_ChunkData.X(); x++)
            {
                if (m_ChunkData.At({x, y, z}).type != BlockType::Air)
                {
                    if (m_ChunkData.LeftType({ x, y, z }) == BlockType::Air)
                        PushLeft({ x, y, z });
                    if (m_ChunkData.RightType({ x, y, z }) == BlockType::Air)
                        PushRight({ x, y, z });
                    if (m_ChunkData.BackType({ x, y, z }) == BlockType::Air)
                        PushBack({ x, y, z });
                    if (m_ChunkData.FrontType({ x, y, z }) == BlockType::Air)
                        PushFront({x, y, z });
                    if (m_ChunkData.BottomType({ x, y, z }) == BlockType::Air)
                        PushBottom({ x, y, z });
                    if (m_ChunkData.TopType({ x, y, z }) == BlockType::Air)
                        PushTop({ x, y, z });
                }
            }
        }
    }
}