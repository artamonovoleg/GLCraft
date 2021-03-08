#pragma once

#include <cstdint>

enum class VoxelType : uint8_t
{
    Air     = 0,
    Grass   = 1,
    Ground  = 2,
    Sand    = 3,
    Glass   = 4
};

struct Voxel
{
    VoxelType type = VoxelType::Air;
};