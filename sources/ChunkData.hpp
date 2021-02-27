#pragma once

#include <array>
#include "Block.hpp"

template<size_t x, size_t y, size_t z>
class ChunkData
{
    private:
        std::array<Block, x * y * z> m_Data;
    public:
        ChunkData() = default;
        ~ChunkData() = default;

        const size_t X() const { return x; }
        const size_t Y() const { return y; }
        const size_t Z() const { return z; }

        const Block& At(const glm::ivec3& pos) const
        {
            return m_Data.at((pos.x * y * z) + (pos.y * x) + pos.z);
        }

        Block& At(const glm::ivec3& pos)
        {
            return m_Data.at((pos.x * y * z) + (pos.y * x) + pos.z);
        }

        const BlockType LeftType(const glm::ivec3& pos) const
        {
            if (pos.x == 0)
                return BlockType::Air;
            else
                return At({ pos.x - 1, pos.y, pos.z }).type;
        };

        const BlockType RightType(const glm::ivec3& pos) const
        {
            if (pos.x == x - 1)
                return BlockType::Air;
            else
                return At({ pos.x + 1, pos.y, pos.z }).type;
        }

        const BlockType BottomType(const glm::ivec3& pos) const
        {
            if (pos.y == 0)
                return BlockType::Air;
            else
                return At({ pos.x, pos.y - 1, pos.z }).type;
        }

        const BlockType TopType(const glm::ivec3& pos) const
        {
            if (pos.y == y - 1)
                return BlockType::Air;
            else
                return At({ pos.x, pos.y + 1, pos.z }).type;
        }

        const BlockType BackType(const glm::ivec3& pos) const
        {
            if (pos.z == 0)
                return BlockType::Air;
            else
                return At({ pos.x, pos.y, pos.z - 1 }).type;
        }

        const BlockType FrontType(const glm::ivec3& pos) const
        {
            if (pos.z == z - 1)
                return BlockType::Air;
            else
                return At({ pos.x, pos.y, pos.z + 1 }).type;
        }
};