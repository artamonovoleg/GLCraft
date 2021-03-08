#pragma once

#include <glm/glm.hpp>

using VoxelPosition = glm::ivec3;

glm::ivec3 LocalToGlobalVoxel(const VoxelPosition& chunkPos, const VoxelPosition& position);