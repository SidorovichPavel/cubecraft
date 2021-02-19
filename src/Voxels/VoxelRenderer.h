#pragma once

#include <vector>
#include <src/Mesh/Mesh.h>
#include <src/Voxels/Chunk.h>

class VoxelRenderer
{
	std::vector<float> mBuffer;
public:
	VoxelRenderer(size_t _Capacity);
	~VoxelRenderer();
	tgl::Mesh* render(const Chunk& _Chunk);
};

