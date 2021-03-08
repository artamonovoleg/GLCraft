#pragma once

#include <glm/glm.hpp>

using GlobalPosition = glm::vec3;
using VoxelPosition = glm::ivec3;

VoxelPosition GlobalToVoxel(const GlobalPosition& position);
VoxelPosition LocalToGlobalVoxel(const VoxelPosition& chunkPos, const VoxelPosition& position);
VoxelPosition GlobalToLocalVoxel(const VoxelPosition& position);
VoxelPosition ToChunkPosition(const VoxelPosition& position);