#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

enum class Voxel : uint8_t
{
    Air     = 0,
    Grass   = 1,
    Ground  = 2,
    Glass   = 3
};

using SpriteID = uint8_t;

enum class Face
{
    Left,
    Right,
    Bottom,
    Top,
    Front,
    Back
};

struct VoxelData
{
    bool solid = true;
    std::array<SpriteID, 6> sprites;
};

class VoxelDataManager
{
    private:
        std::unordered_map<Voxel, VoxelData>    m_SpriteData;
        std::array<std::vector<glm::ivec3>, 6>  m_VerticesData;
    public:
        VoxelDataManager();

        const std::vector<glm::ivec3>& GetVertices(Face face) const;
        std::pair<float, float> GetUV(Voxel voxel, Face face) const;
};
