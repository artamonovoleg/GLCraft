#include "Coordinates.hpp"
#include "Constants.hpp"

VoxelPosition ToChunkPosition(const VoxelPosition& position)
{
    return {
            (position.x < 0) ? (position.x - ChunkSize) / ChunkSize : position.x / ChunkSize,
            (position.y < 0) ? (position.y - ChunkSize) / ChunkSize : position.y / ChunkSize,
            (position.z < 0) ? (position.z - ChunkSize) / ChunkSize : position.z / ChunkSize,
            };
}