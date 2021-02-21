#pragma once

#include <src/Voxels/Chunk.h>
#include <vector>

namespace voxel
{
	namespace hide
	{
		using Chunk = ::voxel::Chunk;

		struct z_place;

		struct yz_place
		{
			uint32_t x, width, height, depth;
			::voxel::Chunk* Data;

			yz_place(::voxel::Chunk* _Ptr, const uint32_t _x, const uint32_t _w, const uint32_t _h, const uint32_t _d) noexcept;
			~yz_place() noexcept;

			z_place operator[](uint32_t y) noexcept;
			const z_place operator[](uint32_t y) const noexcept;
		};

		struct z_place
		{
			uint32_t x, y, width, height, depth;
			::voxel::Chunk* Data;

			z_place(::voxel::Chunk* _Ptr, const uint32_t _x, const uint32_t _y, const uint32_t _w, const uint32_t _h, const uint32_t _d) noexcept;
			~z_place();

			::voxel::Chunk& operator[](uint32_t z) noexcept;
			const ::voxel::Chunk& operator[](uint32_t z) const noexcept;
		};
	}

	class Chunks : public std::vector<Chunk>
	{
		using base = std::vector<Chunk>;
	public:
		Chunks(int32_t width, int32_t heigth, int32_t depth);
		~Chunks();

		void set_neightbors(int32_t _Width, int32_t _Height, int32_t _Depth) noexcept;

		hide::yz_place operator[](uint32_t x) noexcept;
		const hide::yz_place operator[](uint32_t x) const noexcept;
	private:
		const uint32_t mWidth, mHeight, mDepth;
	};
}