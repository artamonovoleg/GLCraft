#include "Voxel.hpp"
#include "Constants.hpp"

VoxelDataManager::VoxelDataManager()
{
    m_VerticesData[static_cast<int>(Face::Left)]    = { glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 1.0), glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0) };
    m_VerticesData[static_cast<int>(Face::Right)]   = { glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0) };
    m_VerticesData[static_cast<int>(Face::Bottom)]  = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 1.0) };
    m_VerticesData[static_cast<int>(Face::Top)]     = { glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(1.0, 1.0, 1.0) };
    m_VerticesData[static_cast<int>(Face::Front)]   = { glm::ivec3(0.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 1.0) };
    m_VerticesData[static_cast<int>(Face::Back)]    = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 0.0) };

    m_SpriteData[Voxel::Air].sprites    = { 0, 0, 0, 0, 0, 0 };
    m_SpriteData[Voxel::Grass].sprites  = { 1, 1, 1, 1, 1, 1 };
    m_SpriteData[Voxel::Ground].sprites = { 2, 2, 2, 2, 2, 2 };
    m_SpriteData[Voxel::Sand].sprites   = { 3, 3, 3, 3, 3, 3 };
    m_SpriteData[Voxel::Glass].sprites  = { 4, 4, 4, 4, 4, 4 };

    m_Transparent.insert(Voxel::Glass);
}

const std::vector<glm::ivec3>& VoxelDataManager::GetVertices(Face face) const
{
    return m_VerticesData.at(static_cast<int>(face));
}

std::pair<float, float> VoxelDataManager::GetUV(Voxel voxel, Face face) const
{
    SpriteID sprite = m_SpriteData.at(voxel).sprites.at(static_cast<int>(face));
    float u = (sprite % SpritesInSide) * UVSize;
    float v = (sprite / SpritesInSide) * UVSize;
    return { u, v };
}

bool VoxelDataManager::IsTransparent(Voxel voxel) const
{
    return (m_Transparent.find(voxel) != m_Transparent.cend());
}