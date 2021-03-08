#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "Voxel.hpp"

class SpriteLibrary
{
    private:
        float m_SpritesInSide;
        std::unordered_map<voxelType, glm::ivec2> m_Sprites;

        /// return left up sprite corner
        glm::vec2 GetSprite(const glm::ivec2& pos);
    public:
        SpriteLibrary();
        ~SpriteLibrary() = default;
        
        glm::vec2 GetSprite(voxelType id);
        float GetSpriteOffset() const { return m_SpritesInSide; }
};