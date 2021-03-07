#pragma once

#include <glm/glm.hpp>

class Voxel;

struct RaycastResult
{
    glm::ivec3 norm;
    glm::ivec3 end;
    Voxel* voxel;
};