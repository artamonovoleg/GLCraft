#include "Coordinates.hpp"

glm::ivec3 LocalToGlobalVoxel(const VoxelPosition& chunkPos, const VoxelPosition& position)
{
    return (position - chunkPos);
}