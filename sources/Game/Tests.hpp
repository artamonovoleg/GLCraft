#include <iostream>
#include "Coordinates.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
#include "ChunkManager.hpp"

static void RunTest()
{
    /// Convert to chunk position
    assert(ToChunkPosition({ 0, 0, 2 }) == ChunkPosition(0, 0, 0));
    assert(ToChunkPosition({ 0, 0, -3 }) == ChunkPosition(0, 0, -1));
}