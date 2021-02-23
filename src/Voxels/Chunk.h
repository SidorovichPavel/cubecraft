#pragma once

#include <src/Voxels/Voxel.h>
#include <src/Mesh/Mesh.h>
#include <src/linearAlgebra.h>
#include <vector>
#include <array>

namespace voxel
{
	constexpr auto CHUNK_WIDTH = 16;
	constexpr auto CHUNK_HEIGHT = 128;
	constexpr auto CHUNK_DEPTH = 16;
	constexpr auto VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

	enum class DrawMode
	{
		NoDraw,
		SingleCore,
		MultiCore,
	};

	constexpr auto gVertexSize = (3 + 3 + 2);
	#define attrib_pack 3,3,2

	class Chunk
	{
		using vox_array = std::vector<voxel>;
		vox_array mVoxels;
		std::array<Chunk*, 6> mNeighbors;

		static std::vector<float> staticBuffer;
		static std::vector<uint32_t> staticIndices;
		static const std::vector<la::vec3i> staticDirections;
		static const std::vector<std::vector<std::array<float, gVertexSize>>> staticOffsets;
	public:
		int32_t mGlobalX, mGlobalY, mGlobalZ;
		tgl::Mesh* mMesh;

		Chunk(int32_t _GX = 0, int32_t _GY = 0, int32_t _GZ = 0);
		~Chunk();

		static void push_data(const std::array<float, gVertexSize * 4>& _VertexData);
		static void render(Chunk& _Chunk);

		void set_neighbors(const Chunk* _R, const Chunk* _L,
						   const Chunk* _U, const Chunk* _D,
						   const Chunk* _F, const Chunk* _B);
		void draw(uint32_t _ObjType = GL_TRIANGLES);

		voxel& operator[](size_t _Index) noexcept;
		voxel operator[](size_t _Index) const noexcept;
	};

}