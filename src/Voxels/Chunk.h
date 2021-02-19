#pragma once

#include <src/Voxels/Voxel.h>
#include <vector>

constexpr auto CHUNK_WIDTH = 16;
constexpr auto CHUNK_HEIGHT = 16;
constexpr auto CHUNK_DEPTH = 16;
constexpr auto VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

class Chunk
{
	std::vector<voxel> mVoxels;

public:
	Chunk();
	~Chunk();

	voxel& operator[](size_t _Index) noexcept;
	voxel operator[](size_t _Index) const noexcept;
};