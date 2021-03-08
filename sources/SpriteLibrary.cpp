#include "SpriteLibrary.hpp"

SpriteLibrary::SpriteLibrary()
    : m_SpritesInSide(16.0f)
{
    /// coord in atlas. 0 0 - left up
    m_Sprites[VoxelType::Grass]   = { 0, 0 };
    m_Sprites[VoxelType::Ground]  = { 1, 0 };
    m_Sprites[VoxelType::Sand]    = { 2, 0 };
    m_Sprites[VoxelType::Glass]   = { 3, 0 };
}

glm::vec2 SpriteLibrary::GetSprite(const glm::ivec2& pos)
{
    return { pos.x / m_SpritesInSide, pos.y / m_SpritesInSide };
}

glm::vec2 SpriteLibrary::GetSprite(voxelType id)
{
    return GetSprite(m_Sprites.at(id));
}