#pragma once

#include <src/linearAlgebra.h>
#include <src/tinyGL.h>
#include <vector>


class LineBatch
{
	tgl::Mesh* mMesh;
	std::vector<float> mBuffer;
	std::vector<unsigned> mIndices;

	static constexpr auto gVertexSize = (3 + 4);
	#define attribs_pack 3,4

	static std::vector<la::vec3> staticDirections;

	void push_v3(const la::vec3& _V);
	void push_v4(const la::vec4& _V);
public:
	LineBatch(size_t _Capacity);
	void line(const la::vec3& _Pos1, const la::vec3& _Pos2, const la::vec4& _Color);
	void box(const la::vec3& _Pos, const la::vec3& _Size, const la::vec4& _Color);
	void render();
	void draw();
};