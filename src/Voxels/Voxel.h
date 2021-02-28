#pragma once

#include <stdint.h>
#include <vector>

namespace voxel
{
	struct Voxel;

	namespace hide
	{
		struct vox_ref_z_place;

		struct vox_ref_yz_place
		{
			Voxel* mData;
			uint32_t mWidth, mHeight, mDepth, x;

			vox_ref_yz_place(Voxel* _Data, const uint32_t _X, const uint32_t _W, const uint32_t _H, const uint32_t _D) noexcept;

			vox_ref_z_place operator[](uint32_t _Y) noexcept;
			const vox_ref_z_place operator[](uint32_t _Y) const noexcept;
		};

		struct vox_ref_z_place
		{
			Voxel* mData;
			uint32_t mWidth, mHeight, mDepth, x, y;

			vox_ref_z_place(Voxel* _Data, const uint32_t _X, const uint32_t _Y, const uint32_t _W, const uint32_t _H, const uint32_t _D) noexcept;

			Voxel& operator[](uint32_t _Z) noexcept;
			const Voxel& operator[](uint32_t _Z) const noexcept;
		};



	}

	struct Voxel
	{
		uint8_t mType;
	};

	class Voxels : public std::vector<Voxel>
	{
		using base = std::vector<Voxel>;
	public:
		Voxels(uint32_t _Width, uint32_t _Height, uint32_t _Depth);
		~Voxels();

		hide::vox_ref_yz_place operator[](uint32_t _X) noexcept;
		const hide::vox_ref_yz_place operator[](uint32_t _X) const noexcept;
	private:
		const uint32_t mWidth, mHeight, mDepth;
	};

}