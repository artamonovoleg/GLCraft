#pragma once

#include <memory>
#include <unordered_set>
#include <glm/glm.hpp>
#include "Voxel.hpp"

class VoxelData
{
    private:
        std::unordered_set<VoxelType>   m_Transparents;
    public:
        VoxelData();
        ~VoxelData() = default;

        bool IsTransparent(const Voxel& voxel);
};