#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

using VoxelPosition = glm::ivec3;

/// Convert world coordinates to voxel coordinates
VoxelPosition ToVoxelPosition(const glm::vec3& position);

/// Convert world voxel position to chunk position
VoxelPosition ToChunkPosition(const VoxelPosition& position);

VoxelPosition GlobalVoxelToLocal(const VoxelPosition& position);
VoxelPosition LocalVoxelToGlobal(const VoxelPosition& chunkPosition, const VoxelPosition& voxelPosition);