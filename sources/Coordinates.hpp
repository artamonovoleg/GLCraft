#pragma once

#include <glm/glm.hpp>

static const int CHUNK_SIDE = 16;

static glm::ivec3 GlobalToVoxel(const glm::vec3& position)
{
    auto x = static_cast<int>(std::floor(position.x));
    auto y = static_cast<int>(std::floor(position.y));
    auto z = static_cast<int>(std::floor(position.z));
    return { x, y, z };
}

static glm::ivec3 GlobalToChunkPosition(const glm::ivec3& position)
{
    return {
        position.x < 0 ? ((position.x - CHUNK_SIDE) / CHUNK_SIDE) : (position.x / CHUNK_SIDE),
        0,
        position.z < 0 ? ((position.z - CHUNK_SIDE) / CHUNK_SIDE) : (position.z / CHUNK_SIDE),
    };
}

static glm::ivec3 GlobalVoxelToLocal(const glm::ivec3& position)
{
    return {(CHUNK_SIDE + (position.x % CHUNK_SIDE)) % CHUNK_SIDE,
            position.y,
            (CHUNK_SIDE + (position.z % CHUNK_SIDE)) % CHUNK_SIDE };
}