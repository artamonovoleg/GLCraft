#include "SpriteLibrary.hpp"

SpriteLibrary::SpriteLibrary()
    : m_SpritesInSide(16.0f)
{
    /// coord in atlas. 0 0 - left up
    m_Sprites[VoxelID::Grass]   = { 0, 0 };
    m_Sprites[VoxelID::Ground]  = { 1, 0 };
    m_Sprites[VoxelID::Sand]    = { 2, 0 };
    m_Sprites[VoxelID::Glass]   = { 3, 0 };
}

glm::vec2 SpriteLibrary::GetSprite(const glm::ivec2& pos)
{
    return { pos.x / m_SpritesInSide, pos.y / m_SpritesInSide };
}

glm::vec2 SpriteLibrary::GetSprite(voxelID id)
{
    return GetSprite(m_Sprites.at(id));
}