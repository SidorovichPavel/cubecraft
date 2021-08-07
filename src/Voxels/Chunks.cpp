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
			assert(y < height);
			return z_place(Data, x, y, width, height, depth);
		}

		const z_place yz_place::operator[](uint32_t y) const noexcept
		{
			assert(y < height);
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
						right = &(*this)[x + 1][y][z];
					if (x > 0)
						left = &(*this)[x - 1][y][z];
					if (z < _Depth - 1)
						front = &(*this)[x][y][z + 1];
					if (z > 0)
						back = &(*this)[x][y][z - 1];

					(*this)[x][y][z].set_neighbors(right, left, nullptr, nullptr, front, back);
				}
	}

	Voxel* Chunks::get(int32_t _X, int32_t _Y, int32_t _Z) noexcept
	{
		auto zero_chunk = base::front();

		auto x = _X >> Chunk::WIDTH_SHIFT;
		auto y = _Y >> Chunk::HEIGHT_SHIFT;
		auto z = _Z >> Chunk::DEPTH_SHIFT;

		auto cx = x - zero_chunk.x;
		auto cy = y - zero_chunk.y;
		auto cz = z - zero_chunk.z;

		if (cx < 0 || cx >= static_cast<int32_t>(mWidth) ||
			cy < 0 || cy >= static_cast<int32_t>(mHeight) ||
			cz < 0 || cz >= static_cast<int32_t>(mDepth))
			return nullptr;

		auto lx = _X - (x << Chunk::WIDTH_SHIFT);
		auto ly = _Y - (y << Chunk::HEIGHT_SHIFT);
		auto lz = _Z - (z << Chunk::DEPTH_SHIFT);

		if (lx < 0 || lx >= Chunk::WIDTH || 
			ly < 0 || ly >= Chunk::HEIGHT || 
			lz < 0 || lz >= Chunk::DEPTH)
			return nullptr;

		decltype(auto) chunk = (*this)[cx][cy][cz];

		return &chunk[lx][ly][lz];
	}

	void Chunks::set(int32_t _X, int32_t _Y, int32_t _Z, uint8_t _ID) noexcept
	{
		auto zero_chunk = base::front();

		auto x = _X >> Chunk::WIDTH_SHIFT;
		auto y = _Y >> Chunk::HEIGHT_SHIFT;
		auto z = _Z >> Chunk::DEPTH_SHIFT;

		auto cx = x - zero_chunk.x;
		auto cy = y - zero_chunk.y;
		auto cz = z - zero_chunk.z;

		if (cx < 0 || cx >= static_cast<int32_t>(mWidth) ||
			cy < 0 || y >= static_cast<int32_t>(mHeight) ||
			cz < 0 || cz >= static_cast<int32_t>(mDepth))
			return;

		auto lx = _X - (x << Chunk::WIDTH_SHIFT);
		auto ly = _Y - (y << Chunk::HEIGHT_SHIFT);
		auto lz = _Z - (z << Chunk::DEPTH_SHIFT);

		if (lx < 0 || lx >= Chunk::WIDTH || ly < 0 || ly >= Chunk::HEIGHT || lz < 0 || lz >= Chunk::DEPTH)
			return;

		decltype(auto) chunk = (*this)[cx][cy][cz];
		chunk[lx][ly][lz].mType = _ID;

		chunk.mNeedUpdate = true;
		if (lx == Chunk::WIDTH - 1 &&
			chunk.mNeighbors[0])		chunk.mNeighbors[0]->mNeedUpdate = true;
		if (lx == 0 &&
			chunk.mNeighbors[1])		chunk.mNeighbors[1]->mNeedUpdate = true;
		if (ly == Chunk::HEIGHT - 1 &&
			chunk.mNeighbors[2])		chunk.mNeighbors[2]->mNeedUpdate = true;
		if (ly == 0 &&
			chunk.mNeighbors[3])		chunk.mNeighbors[3]->mNeedUpdate = true;
		if (lz == Chunk::DEPTH - 1 &&
			chunk.mNeighbors[4])		chunk.mNeighbors[4]->mNeedUpdate = true;
		if (lz == 0 &&
			chunk.mNeighbors[5])		chunk.mNeighbors[5]->mNeedUpdate = true;
	}

	Voxel* Chunks::ray_cast(const glm::vec3& src, const glm::vec3& dir,
							float maxDist,
							glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
	{
		float px = src.x;
		float py = src.y;
		float pz = src.z;

		float dx = dir.x;
		float dy = dir.y;
		float dz = dir.z;

		float t = 0.0f;
		int ix = static_cast<int>(floor(px));
		int iy = static_cast<int>(floor(py));
		int iz = static_cast<int>(floor(pz));

		float stepx = (dx > 0.0f) ? 1.f : -1.f;
		float stepy = (dy > 0.0f) ? 1.f : -1.f;
		float stepz = (dz > 0.0f) ? 1.f : -1.f;

		constexpr float infinity = std::numeric_limits<float>::infinity();

		float txDelta = (dx == 0.0f) ? infinity : abs(1.f/dx);
		float tyDelta = (dy == 0.0f) ? infinity : abs(1.f/dy);
		float tzDelta = (dz == 0.0f) ? infinity : abs(1.f/dz);

		float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
		float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
		float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

		float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
		float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
		float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

		int steppedIndex = -1;

		while (t <= maxDist) {
			auto voxel = get(ix, iy, iz);
			if (voxel == nullptr || voxel->mType) {
				end[0] = px + t * dx;
				end[1] = py + t * dy;
				end[2] = pz + t * dz;

				iend[0] = static_cast<float>(ix);
				iend[1] = static_cast<float>(iy);
				iend[2] = static_cast<float>(iz);

				norm[0] = norm[1] = norm[2] = 0.0f;
				if (steppedIndex == 0) norm[0] = -stepx;
				if (steppedIndex == 1) norm[1] = -stepy;
				if (steppedIndex == 2) norm[2] = -stepz;
				return voxel;
			}
			if (txMax < tyMax) {
				if (txMax < tzMax) {
					ix += static_cast<int>(stepx);
					t = txMax;
					txMax += txDelta;
					steppedIndex = 0;
				}
				else {
					iz += static_cast<int>(stepz);
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
			else {
				if (tyMax < tzMax) {
					iy += static_cast<int>(stepy);
					t = tyMax;
					tyMax += tyDelta;
					steppedIndex = 1;
				}
				else {
					iz += static_cast<int>(stepz);
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
		}
		iend[0] = static_cast<float>(ix);
		iend[1] = static_cast<float>(iy);
		iend[2] = static_cast<float>(iz);

		end[0] = px + t * dx;
		end[1] = py + t * dy;
		end[2] = pz + t * dz;
		norm[0] = norm[1] = norm[2] = 0.0f;
		return nullptr;
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