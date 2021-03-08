#pragma once

#include <vector>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Mesh.hpp"

class ChunkManager;
class Chunk;

class MeshBuilder
{
    private:
        std::unordered_map<VoxelPosition, Mesh> m_Meshes;
        ChunkManager& m_Manager;

        void PushIndices(std::vector<unsigned int>& indices);
        void PushFace(const VoxelPosition& position, Mesh& mesh, VoxelType type, const std::vector<glm::ivec3>& face);
        void GenerateChunkMesh(const Chunk& chunk, Mesh& mesh);

        void UpdateChunk(const Chunk& chunk);
    public:
        MeshBuilder(ChunkManager& manager);

        void Update();
        
        const std::unordered_map<VoxelPosition, Mesh>& GetMeshes() const;
};