#include "Coordinates.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
#include "ChunkManager.hpp"

static void RunTest()
{
    ChunkManager manager(Camera({ 0, 0, 0 }));
    manager.AddChunk({ 0, 0, -1 });
    assert(manager.HasChunk({ 0, 0, -1 }));
    auto position = LocalVoxelToGlobal({ 0, 0, -1 }, { 1, 1, 0 });
    assert(position == VoxelPosition(1, 1, -3));
    assert(ToChunkPosition(position) == VoxelPosition(0, 0, -1));
    std::cout << ToChunkPosition(position).z << std::endl;
    manager.SetVoxel(position, Voxel::Ground);
    assert(manager.GetVoxel(position) == Voxel::Ground);
    auto voxel = manager.GetChunkMap().at({ 0, 0, -1 }).QGetVoxel({ 1, 1, 0 });
    assert(voxel == Voxel::Ground);
    assert(manager.GetChunkMap().size() == 1);
}