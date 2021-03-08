#pragma once

#include <glm/glm.hpp>

using VoxelPosition = glm::ivec3;

glm::ivec3 LocalToGlobalVoxel(const VoxelPosition& chunkPos, const VoxelPosition& position);
glm::ivec3 GlobalToLocalVoxel(const VoxelPosition& position);
glm::ivec3 ToChunkPosition(const VoxelPosition& position);