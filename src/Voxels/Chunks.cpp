#include <src/Voxels/Chunks.h>

namespace voxel
{
	namespace hide
	{
		//yz_place
		yz_place::yz_place(::voxel::Chunk* _Ptr, const uint32_t _x, const uint32_t _w, const uint32_t _h, const uint32_t _d) noexcept
			:
			Data(_Ptr),
			x(_x),
			width(_w), height(_h), depth(_d)
		{}

		yz_place::~yz_place() noexcept
		{}

		z_place yz_place::operator[](uint32_t y) noexcept
		{
			return z_place(Data, x, y, width, height, depth);
		}

		const z_place yz_place::operator[](uint32_t y) const noexcept
		{
			return z_place(Data, x, y, width, height, depth);
		}
		//end

		//z_place
		z_place::z_place(::voxel::Chunk* _Ptr, const uint32_t _x, const uint32_t _y, const uint32_t _w, const uint32_t _h, const uint32_t _d) noexcept
			:
			Data(_Ptr),
			x(_x), y(_y),
			width(_w), height(_h), depth(_d)
		{}

		z_place::~z_place()
		{}

		::voxel::Chunk& z_place::operator[](uint32_t z) noexcept
		{
			uint64_t offset = static_cast<uint64_t>(y) * depth + z;
			offset *= width;
			offset += x;
			return *(Data + offset);
		}

		const::voxel::Chunk& z_place::operator[](uint32_t z) const noexcept
		{
			uint64_t offset = static_cast<uint64_t>(y) * depth + z;
			offset *= width;
			offset += x;
			return *(Data + offset);
		}
		//end
	}

	Chunks::Chunks(int32_t width, int32_t heigth, int32_t depth)
		:
		base(),
		mWidth(width), mHeight(heigth), mDepth(depth)
	{
		base::reserve(static_cast<uint64_t>(width) * heigth * depth);
	}

	Chunks::~Chunks()
	{}

	void Chunks::set_neightbors(int32_t _Width, int32_t _Height, int32_t _Depth) noexcept
	{
		for (auto y = 0; y < _Height; ++y)
			for (auto z = 0; z < _Depth; ++z)
				for (auto x = 0; x < _Width; ++x)
				{
					::voxel::Chunk* right, * left, * front, * back;
					right = left = front = back = nullptr;

					if (x < _Width - 1)
						right = &(*this)[x + 1][0][z];
					if (x > 0)
						left = &(*this)[x - 1][0][z];
					if (z < _Depth - 1)
						front = &(*this)[x][0][z + 1];
					if (z > 0)
						back = &(*this)[x][0][z - 1];

					(*this)[x][0][z].set_neighbors(right, left, nullptr, nullptr, front, back);
				}
	}

	hide::yz_place Chunks::operator[](uint32_t x) noexcept
	{
		return hide::yz_place(base::data(), x, mWidth, mHeight, mDepth);
	}
	const hide::yz_place Chunks::operator[](uint32_t x) const noexcept
	{
		return hide::yz_place(const_cast<Chunk*>(base::data()), x, mWidth, mHeight, mDepth);
	}
}