#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

using VoxelPosition = glm::ivec3;

VoxelPosition ToChunkPosition(const VoxelPosition& position);