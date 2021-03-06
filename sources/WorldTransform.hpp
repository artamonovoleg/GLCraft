#pragma once

#include <glm/glm.hpp>

static const int CHUNK_SIZE = 16;

static glm::ivec3 GlobalToVoxel(const glm::vec3& position)
{
    auto x = static_cast<int>(std::floor(position.x));
    auto y = static_cast<int>(std::floor(position.y));
    auto z = static_cast<int>(std::floor(position.z));
    return { x, y, z };
}

static glm::ivec3 ToLocalVoxelPosition(const glm::ivec3& position)
{
    return { (CHUNK_SIZE + (position.x % CHUNK_SIZE)) % CHUNK_SIZE,
                position.y,
            (CHUNK_SIZE + (position.z % CHUNK_SIZE)) % CHUNK_SIZE };
}