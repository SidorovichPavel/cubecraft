#include "VoxelRenderer.h"
#include <algorithm>
#include <iterator>

namespace voxel
{

	void VoxelRenderer::push_data(const std::array<float, gVertexSize * 4>& _VertexData)
	{
		unsigned index = static_cast<unsigned>(mBuffer.size()) / _gVertexSize;

		std::copy(_VertexData.begin(), _VertexData.end(), std::back_insert_iterator<std::vector<float>>(mBuffer));

		mIndices.push_back(index + 0);
		mIndices.push_back(index + 1);
		mIndices.push_back(index + 2);
		mIndices.push_back(index + 2);
		mIndices.push_back(index + 3);
		mIndices.push_back(index + 0);
	}

	VoxelRenderer::VoxelRenderer(size_t _Capacity)
	{
		mBuffer.reserve(_Capacity);
		mIndices.reserve(_Capacity / 2 + _Capacity);
	}

	VoxelRenderer::~VoxelRenderer()
	{}

	tgl::Mesh* VoxelRenderer::render(const Chunk& _Chunk)
	{
		decltype(auto) chunk = _Chunk;

		std::array<float, 4 * gVertexSize> vertex_data;

		for (auto y = 0; y < CHUNK_HEIGHT; ++y)
			for (auto z = 0; z < CHUNK_DEPTH; ++z)
				for (auto x = 0; x < CHUNK_WIDTH; ++x)
				{
					auto voxel = _Chunk[(y * CHUNK_DEPTH + z) * CHUNK_WIDTH + x];
					if (!voxel.mType)
						continue;

					for (auto i = 0; i < 6; ++i)
					{
						auto
							xx = x + mDirections[i][0],
							yy = y + mDirections[i][1],
							zz = z + mDirections[i][2];

						if ((xx < 0 || xx >= CHUNK_WIDTH || yy < 0 || yy >= CHUNK_HEIGHT || zz < 0 || zz >= CHUNK_DEPTH) ||
							!chunk[(yy * CHUNK_DEPTH + zz) * CHUNK_WIDTH + xx].mType)
						{
							for (auto counter = 0; counter < 4; ++counter)
							{
								vertex_data[counter * _gVertexSize + 0] = x + mOffsets[i][counter][0];
								vertex_data[counter * _gVertexSize + 1] = y + mOffsets[i][counter][1];
								vertex_data[counter * _gVertexSize + 2] = z + mOffsets[i][counter][2];
								vertex_data[counter * _gVertexSize + 3] = mOffsets[i][counter][3];
								vertex_data[counter * _gVertexSize + 4] = mOffsets[i][counter][4];
							}
							push_data(vertex_data);
						}
					}

				}

		auto result = new tgl::Mesh;
		result->bind();
		result->add_attribut<3, 2>(mBuffer.size(), mBuffer.data());
		result->set_indices(mIndices.size(), mIndices.data());
		return result;
	}

}