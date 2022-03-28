#pragma once

#include "Voxel.hpp"

#include <Mesh/Mesh.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace voxel
{
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
		Voxels mVoxels;

		tgl::Mesh* mMesh;

		static std::vector<float> staticBuffer;
		static std::vector<uint32_t> staticIndices;
		static const std::vector<glm::ivec3> staticDirections;
		static const std::vector<std::vector<std::array<float, gVertexSize>>> staticOffsets;
	public:
		static const int WIDTH_SHIFT;
		static const int HEIGHT_SHIFT;
		static const int DEPTH_SHIFT;
		
		static const int WIDTH;
		static const int HEIGHT;
		static const int DEPTH;

		static const int VOLUME;

		int32_t mGlobalX, mGlobalY, mGlobalZ;
		int32_t x, y, z;
		std::array<Chunk*, 6> mNeighbors;
		bool mNeedUpdate;

		Chunk(int32_t _GX = 0, int32_t _GY = 0, int32_t _GZ = 0);
		~Chunk();

		static void push_data(const std::array<float, gVertexSize * 4>& _VertexData);
		static void render(Chunk& _Chunk);

		void set_neighbors(const Chunk* _R, const Chunk* _L,
						   const Chunk* _U, const Chunk* _D,
						   const Chunk* _F, const Chunk* _B);
		void draw(uint32_t _ObjType = GL_TRIANGLES);

		hide::vox_ref_yz_place operator[](uint32_t _X) noexcept;
		const hide::vox_ref_yz_place operator[](uint32_t _X) const noexcept;
	};
}