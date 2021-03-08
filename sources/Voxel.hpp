#pragma once

#include <cstdint>

using voxelType = uint8_t;

namespace VoxelType
{
    enum : voxelType
    {
        Air     = 0,
        Grass   = 1,
        Ground  = 2,
        Sand    = 3,
        Glass   = 4
    };
};

class Voxel
{
    voxelType   id              = VoxelType::Air;
    bool        isTransparent   = true;
};