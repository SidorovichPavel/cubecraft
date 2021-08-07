#pragma once

#include <src/glm/glm.hpp>
#include <src/Mesh/Mesh.h>
#include <vector>


class LineBatch
{
	tgl::Mesh* mMesh;
	std::vector<float> mBuffer;
	std::vector<unsigned> mIndices;

	static constexpr auto gVertexSize = (3 + 4);
	#define attribs_pack 3,4

	static std::vector<glm::vec3> staticDirections;

	void push_v3(const glm::vec3& _V);
	void push_v4(const glm::vec4& _V);
public:
	LineBatch(size_t _Capacity);
	void line(const glm::vec3& _Pos1, const glm::vec3& _Pos2, const glm::vec4& _Color);
	void box(const glm::vec3& _Pos, const glm::vec3& _Size, const glm::vec4& _Color);
	void render();
	void draw();
};