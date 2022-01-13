#include "Voxel.hpp"

#include <assert.h>

namespace voxel
{
	namespace hide
	{
		vox_ref_yz_place::vox_ref_yz_place(Voxel* _Data, const uint32_t _X, const uint32_t _W, const uint32_t _H, const uint32_t _D) noexcept
			:
			mData(_Data),
			x(_X),
			mWidth(_W), mHeight(_H), mDepth(_D)
		{}

		vox_ref_z_place vox_ref_yz_place::operator[](uint32_t _Y) noexcept
		{
			assert(_Y < mHeight);
			return vox_ref_z_place(mData, x, _Y, mWidth, mHeight, mDepth);
		}
		const vox_ref_z_place vox_ref_yz_place::operator[](uint32_t _Y) const noexcept
		{
			assert(_Y < mHeight);
			return vox_ref_z_place(mData, x, _Y, mWidth, mHeight, mDepth);
		}

		vox_ref_z_place::vox_ref_z_place(Voxel* _Data, const uint32_t _X, const uint32_t _Y, const uint32_t _W, const uint32_t _H, const uint32_t _D) noexcept
		:
			mData(_Data),
			x(_X), y(_Y),
			mWidth(_W), mHeight(_H), mDepth(_D)
		{}

		Voxel& vox_ref_z_place::operator[](uint32_t _Z) noexcept
		{
			assert(_Z < mDepth);
			uint64_t offset = static_cast<uint64_t>(y) * mDepth + _Z;
			offset *= mWidth;
			offset += x;
			return *(mData + offset);
		}
		const Voxel& vox_ref_z_place::operator[](uint32_t _Z) const noexcept
		{
			assert(_Z < mDepth);
			uint64_t offset = static_cast<uint64_t>(y) * mDepth + _Z;
			offset *= mWidth;
			offset += x;
			return *(mData + offset);
		}
	}

	Voxels::Voxels(uint32_t _Width, uint32_t _Height, uint32_t _Depth)
		:
		base(),
		mWidth(_Width), mHeight(_Height), mDepth(_Depth)
	{
		base::reserve(static_cast<size_t>(mWidth) * mHeight * mDepth);
	}

	Voxels::~Voxels()
	{}

	hide::vox_ref_yz_place Voxels::operator[](uint32_t _X) noexcept
	{
		assert(_X < mWidth);
		return hide::vox_ref_yz_place(base::data(), _X, mWidth, mHeight, mDepth);
	}

	const hide::vox_ref_yz_place Voxels::operator[](uint32_t _X) const noexcept
	{
		assert(_X < mWidth);
		return hide::vox_ref_yz_place(const_cast<Voxel*>(base::data()), _X, mWidth, mHeight, mDepth);
	}

}