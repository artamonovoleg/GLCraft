#include "VoxelData.hpp"

VoxelData::VoxelData()
{
    m_Transparents.insert(VoxelType::Air);
}

bool VoxelData::IsTransparent(const Voxel& voxel)
{
    return (m_Transparents.find(voxel.type) != m_Transparents.end());
}