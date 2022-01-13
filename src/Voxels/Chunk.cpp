#include "Chunk.hpp"
#include "../PerlinNoise/PerlinNoise.hpp"

#include <algorithm>
#include <iterator>

#pragma warning(disable:26451)

namespace voxel
{
	const int Chunk::WIDTH_SHIFT = 4;
	const int Chunk::HEIGHT_SHIFT = 7;
	const int Chunk::DEPTH_SHIFT = 4;
	const int Chunk::WIDTH = 1 << Chunk::WIDTH_SHIFT;
	const int Chunk::HEIGHT = 1 << Chunk::HEIGHT_SHIFT;
	const int Chunk::DEPTH = 1 << Chunk::DEPTH_SHIFT;

	const int Chunk::VOLUME = WIDTH * HEIGHT * DEPTH;

	//debug
	constexpr int V = Chunk::WIDTH * Chunk::HEIGHT * Chunk::DEPTH;
	//

	Chunk::Chunk(int32_t _GX, int32_t _GY, int32_t _GZ)
		:
		x(_GX),
		y(_GY),
		z(_GZ),
		mMesh(new tgl::Mesh()),
		mVoxels(Chunk::WIDTH, Chunk::HEIGHT, Chunk::DEPTH),
		mNeedUpdate(true)
	{
		std::fill(mNeighbors.begin(), mNeighbors.end(), nullptr);
		mVoxels.resize(VOLUME);

		siv::PerlinNoise perlin(std::random_device{});

		mGlobalX = x << Chunk::WIDTH_SHIFT;
		mGlobalY = y << Chunk::HEIGHT_SHIFT;
		mGlobalZ = z << Chunk::DEPTH_SHIFT;

		for (auto z = 0; z < Chunk::DEPTH; ++z)
			for (auto x = 0; x < Chunk::WIDTH; ++x)
			{
				float height =
					static_cast<float>(perlin.normalizedOctaveNoise3D_0_1((mGlobalX + x) * 2.735f, (mGlobalY + z) * 0.235f, (mGlobalZ) * 0.05f, 7));
				height *= 10.f;
				height += 5;
				for (auto y = 0; y < Chunk::HEIGHT; ++y)
				{
					uint8_t id = (mGlobalY + y) <= static_cast<int32_t>(height);

					mVoxels[x][y][z].mType = id;
				}
			}
	}

	Chunk::~Chunk()
	{}

	const std::vector<glm::ivec3> Chunk::staticDirections = {
			{ 1,  0,  0 },
			{-1,  0,  0 },
			{ 0,  1,  0 },
			{ 0, -1,  0 },
			{ 0,  0,  1 },
			{ 0,  0, -1 }
	};
	const std::vector<std::vector<std::array<float, gVertexSize>>> Chunk::staticOffsets = {
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
		{ 0.5f,  0.5f, -0.5f, 0.f, 0.f },
		{ 0.5f, -0.5f, -0.5f, 0.f, 1.f },
		{-0.5f, -0.5f, -0.5f, 1.f, 1.f },
		{-0.5f,  0.5f, -0.5f, 1.f, 0.f },
		}
	};
	std::vector<float> Chunk::staticBuffer;
	std::vector<uint32_t> Chunk::staticIndices;

	void Chunk::push_data(const std::array<float, gVertexSize * 4>& _VertexData)
	{
		unsigned index = static_cast<unsigned>(staticBuffer.size()) / gVertexSize;

		std::copy(_VertexData.begin(), _VertexData.end(), std::back_insert_iterator<std::vector<float>>(staticBuffer));

		staticIndices.push_back(index + 0);
		staticIndices.push_back(index + 1);
		staticIndices.push_back(index + 2);
		staticIndices.push_back(index + 2);
		staticIndices.push_back(index + 3);
		staticIndices.push_back(index + 0);
	}

	void Chunk::render(Chunk& _Chunk)
	{
		staticBuffer.clear();
		staticIndices.clear();

		decltype(auto) chunk = _Chunk;
		if (!chunk.mNeedUpdate)
			return;

		std::array<float, 4 * gVertexSize> vertex_data;

		auto add_to_mesh = [&] (int x, int y, int z, int i)
		{
			glm::vec3 normal;
			normal[0] = static_cast<float>(staticDirections[i][0]);
			normal[1] = static_cast<float>(staticDirections[i][1]);
			normal[2] = static_cast<float>(staticDirections[i][2]);

			for (auto counter = 0; counter < 4; ++counter)
			{
				vertex_data[counter * gVertexSize + 0] = chunk.mGlobalX + x + staticOffsets[i][counter][0];
				vertex_data[counter * gVertexSize + 1] = chunk.mGlobalY + y + staticOffsets[i][counter][1];
				vertex_data[counter * gVertexSize + 2] = chunk.mGlobalZ + z + staticOffsets[i][counter][2];
				vertex_data[counter * gVertexSize + 3] = normal[0];
				vertex_data[counter * gVertexSize + 4] = normal[1];
				vertex_data[counter * gVertexSize + 5] = normal[2];
				vertex_data[counter * gVertexSize + 6] = staticOffsets[i][counter][3];
				vertex_data[counter * gVertexSize + 7] = staticOffsets[i][counter][4];
			}
			push_data(vertex_data);
		};

		for (auto y = 0; y < Chunk::HEIGHT; ++y)
		{
			//int32_t global_lightning_access = true;
			for (auto z = 0; z < Chunk::DEPTH; ++z)
				for (auto x = 0; x < Chunk::WIDTH; ++x)
				{
					auto voxel = chunk[x][y][z];
					if (!voxel.mType)
						continue;

					for (auto i = 0; i < 6; ++i)
					{
						auto
							xx = x + staticDirections[i].x,
							yy = y + staticDirections[i].y,
							zz = z + staticDirections[i].z;

						if (!(xx < 0 || xx >= (1 << Chunk::WIDTH_SHIFT) ||
							  yy < 0 || yy >= (1 << Chunk::HEIGHT_SHIFT) ||
							  zz < 0 || zz >= (1 << Chunk::DEPTH_SHIFT)))
						{
							if (!chunk[xx][yy][zz].mType)
							{
								add_to_mesh(x, y, z, i);
							}
						}
						else
						{
							if (chunk.mNeighbors[i])
							{
								auto select_coord = [] (int coord, int dir)
								{
									switch (dir)
									{
									case -1: return 15;
									case 0: return coord;
									case 1: return 0;
									}
									return coord;
								};

								auto _x = select_coord(x, staticDirections[i][0]);
								auto _y = select_coord(y, staticDirections[i][1]);
								auto _z = select_coord(z, staticDirections[i][2]);

								if (!(*chunk.mNeighbors[i])[_x][_y][_z].mType)
								{
									add_to_mesh(x, y, z, i);
								}
							}
						}
					}
				}
		}

		chunk.mNeedUpdate = false;
		chunk.mMesh->set_attribut<attrib_pack>(staticBuffer.size(), staticBuffer.data(), GL_STATIC_DRAW);
		chunk.mMesh->set_indices(staticIndices.size(), staticIndices.data());
		chunk.mMesh->unbind();
	}

	void Chunk::set_neighbors(const Chunk* _R, const Chunk* _L, const Chunk* _U, const Chunk* _D, const Chunk* _F, const Chunk* _B)
	{
		mNeighbors[0] = const_cast<Chunk*>(_R);
		mNeighbors[1] = const_cast<Chunk*>(_L);
		mNeighbors[2] = const_cast<Chunk*>(_U);
		mNeighbors[3] = const_cast<Chunk*>(_D);
		mNeighbors[4] = const_cast<Chunk*>(_F);
		mNeighbors[5] = const_cast<Chunk*>(_B);
	}

	void Chunk::draw(uint32_t _ObjType)
	{
		mMesh->draw(_ObjType);
	}

	hide::vox_ref_yz_place Chunk::operator[](uint32_t _X) noexcept
	{
		assert(_X < Chunk::WIDTH);
		return mVoxels[_X];
	}

	const hide::vox_ref_yz_place Chunk::operator[](uint32_t _X) const noexcept
	{
		assert(_X < Chunk::WIDTH);
		return mVoxels[_X];
	}
}