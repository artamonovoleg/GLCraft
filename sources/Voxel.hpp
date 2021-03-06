#pragma once

#include <cstdint>

using voxelID = uint8_t;
	
namespace VoxelID
{
    enum : voxelID
    {
        Air = 0,
        Grass = 1
    };
}

struct Voxel
{
    voxelID id;
    Voxel() : id(VoxelID::Air) {}
};