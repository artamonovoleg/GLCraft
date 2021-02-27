#pragma once

enum class BlockType
{
    Air,
    Grass,
    Ground,
};

struct Block
{
    BlockType type;
    Block() : type(BlockType::Air) {}
};