#include <src/Voxels/Chunk.h>

Chunk::Chunk()
{
	mVoxels.resize(VOLUME);
	auto it = mVoxels.begin();
	for (auto y = 0; y < CHUNK_HEIGHT; ++y)
		for (auto z = 0; z < CHUNK_DEPTH; ++z)
			for (auto x = 0; x < CHUNK_WIDTH; ++x, ++it)
				it->mType = y <= 5;
}

Chunk::~Chunk()
{
}

voxel& Chunk::operator[](size_t _Index) noexcept
{
	return mVoxels[_Index];
}

voxel Chunk::operator[](size_t _Index) const noexcept
{
	return mVoxels[_Index];
}
