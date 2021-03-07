#pragma once

#include <memory>
#include <array>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "Chunk.hpp"

class Shader;
class Texture;
class Camera;
class Voxel;

enum class FaceType
{
    Left = 0,
    Right,
    Back,
    Front,
    Bottom,
    Top
};

class World
{
    private:
        std::shared_ptr<Shader>     m_Shader;
        std::shared_ptr<Texture>    m_Texture;

        const Camera& m_Camera;
        std::unordered_map<glm::ivec3, Chunk> m_Data;
    public:
        World(const Camera& camera);

        Chunk* GetChunk(const glm::vec3& position);
        
        Voxel* Get(int ix, int iy, int iz);

        void Draw();

        Voxel* Raycast(const glm::vec3& orig, const glm::vec3& dir, float maxDist,  glm::vec3& norm, glm::vec3& iend);
};