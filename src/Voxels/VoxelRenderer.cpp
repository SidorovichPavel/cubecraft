#include "VoxelRenderer.h"

constexpr auto gVertexSize = (3 + 2);

VoxelRenderer::VoxelRenderer(size_t _Capacity)
{
	mBuffer.resize(_Capacity * 6 * gVertexSize);
}

VoxelRenderer::~VoxelRenderer()
{

}

tgl::Mesh* VoxelRenderer::render(const Chunk& _Chunk)
{


	auto result = new tgl::Mesh();
	
	return result;
}
