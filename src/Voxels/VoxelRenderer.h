#pragma once

#include <vector>
#include <array>
#include <src/Mesh/Mesh.h>
#include <src/Voxels/Chunk.h>
#include <src/linearAlgebra.h>

namespace voxel
{
	constexpr auto _gVertexSize = (3 + 2);

	class VoxelRenderer
	{
		std::vector<float> mBuffer;
		std::vector<unsigned> mIndices;

		void push_data(const std::array<float, gVertexSize * 4>& _VertexData);

		const std::vector<la::vec3i> mDirections = {
			{ 1,  0,  0 },
			{-1,  0,  0 },
			{ 0,  1,  0 },
			{ 0, -1,  0 },
			{ 0,  0,  1 },
			{ 0,  0, -1 }
		};

		const std::vector<std::vector<std::array<float, gVertexSize>>> mOffsets = {
			{
			{ 0.5f,  0.5f,  0.5f, 0.f, 0.f },
			{ 0.5f, -0.5f,  0.5f, 0.f, 1.f },
			{ 0.5f, -0.5f, -0.5f, 1.f, 1.f },
			{ 0.5f,  0.5f, -0.5f, 1.f, 0.f },
			},
			{
			{-0.5f,  0.5f, -0.5f, 0.f, 0.f },
			{-0.5f, -0.5f, -0.5f, 0.f, 1.f },
			{-0.5f, -0.5f,  0.5f, 1.f, 1.f },
			{-0.5f,  0.5f,  0.5f, 1.f, 0.f },
			},
			{
			{-0.5f,  0.5f,  0.5f, 0.f, 0.f },
			{ 0.5f,  0.5f,  0.5f, 0.f, 1.f },
			{ 0.5f,  0.5f, -0.5f, 1.f, 1.f },
			{-0.5f,  0.5f, -0.5f, 1.f, 0.f },
			},
			{
			{-0.5f, -0.5f,  0.5f, 0.f, 0.f },
			{-0.5f, -0.5f, -0.5f, 0.f, 1.f },
			{ 0.5f, -0.5f, -0.5f, 1.f, 1.f },
			{ 0.5f, -0.5f,  0.5f, 1.f, 0.f },
			},
			{
			{-0.5f,  0.5f,  0.5f, 0.f, 0.f },
			{-0.5f, -0.5f,  0.5f, 0.f, 1.f },
			{ 0.5f, -0.5f,  0.5f, 1.f, 1.f },
			{ 0.5f,  0.5f,  0.5f, 1.f, 0.f },
			},
			{
			{-0.5f,  0.5f, -0.5f, 0.f, 0.f },
			{ 0.5f,  0.5f, -0.5f, 0.f, 1.f },
			{ 0.5f, -0.5f, -0.5f, 1.f, 1.f },
			{-0.5f, -0.5f, -0.5f, 1.f, 0.f },
			}
		};

	public:
		VoxelRenderer(size_t _Capacity);
		~VoxelRenderer();
		tgl::Mesh* render(const Chunk& _Chunk);
	};

}