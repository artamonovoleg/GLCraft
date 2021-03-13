#pragma once

#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Mesh.hpp"

class ChunkManager;
class Chunk;

class MeshBuilder
{
    private:
        const ChunkManager& m_ChunkManager;
        VoxelDataManager m_VoxelDataManager;

        std::unordered_map<VoxelPosition, Mesh> m_Meshes;
        void BuildMesh(Mesh& mesh, const Chunk& chunk);
        void PushIndices(std::vector<unsigned int>& indices);
        void PushFace(Mesh& mesh, const VoxelPosition& position, Voxel voxel, Face face);
        
        bool ShouldPushFace(const std::array<Voxel, 6>& neighbours, Voxel voxel, Face face);
        std::array<Voxel, 6> GetNeighbourVoxels(const Chunk& chunk, const VoxelPosition& position);
    public:
        MeshBuilder(const ChunkManager& chunkManager);
        
        void Process();

        const std::unordered_map<VoxelPosition, Mesh>& GetMeshes() const { return m_Meshes; }
};