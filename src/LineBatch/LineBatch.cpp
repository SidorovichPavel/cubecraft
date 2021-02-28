#include <src/LineBatch/LineBatch.h>

std::vector<la::vec3> LineBatch::staticDirections = {
	{ 1.f, 1.f, 1.f },
	{ 1.f,-1.f, 1.f },
	{-1.f,-1.f, 1.f },
	{-1.f, 1.f, 1.f },
	{ 1.f, 1.f,-1.f },
	{ 1.f,-1.f,-1.f },
	{-1.f,-1.f,-1.f },
	{-1.f, 1.f,-1.f },
};


void LineBatch::push_v3(const la::vec3& _V)
{
	mBuffer.push_back(_V[0]);
	mBuffer.push_back(_V[1]);
	mBuffer.push_back(_V[2]);
}

void LineBatch::push_v4(const la::vec4& _V)
{
	mBuffer.push_back(_V[0]);
	mBuffer.push_back(_V[1]);
	mBuffer.push_back(_V[2]);
	mBuffer.push_back(_V[3]);
}

LineBatch::LineBatch(size_t _Capacity)
	:
	mMesh(new tgl::Mesh())
{
	mBuffer.reserve(_Capacity * gVertexSize);
	mBuffer.reserve(_Capacity);
}

void LineBatch::line(const la::vec3& _Pos1, const la::vec3& _Pos2, const la::vec4& _Color)
{
	uint32_t index = static_cast<uint32_t>(mBuffer.size() / gVertexSize);

	push_v3(_Pos1);
	push_v4(_Color);

	push_v3(_Pos2);
	push_v4(_Color);

	mIndices.push_back(index + 0);
	mIndices.push_back(index + 1);
}

void LineBatch::box(const la::vec3& _Pos, const la::vec3& _Size, const la::vec4& _Color)
{
	uint32_t index = static_cast<uint32_t>(mBuffer.size() / gVertexSize);

	la::vec3 size = _Size * 0.5f;

	for (auto i = 0; i < 8; ++i)
	{
		mBuffer.push_back(_Pos[0] + staticDirections[i][0] * size[0]);
		mBuffer.push_back(_Pos[1] + staticDirections[i][1] * size[1]);
		mBuffer.push_back(_Pos[2] + staticDirections[i][2] * size[2]);

		push_v4(_Color);
	}

	mIndices.push_back(index + 0);
	mIndices.push_back(index + 1);
	mIndices.push_back(index + 1);
	mIndices.push_back(index + 2);
	mIndices.push_back(index + 2);
	mIndices.push_back(index + 3);
	mIndices.push_back(index + 3);
	mIndices.push_back(index + 0);

	mIndices.push_back(index + 4);
	mIndices.push_back(index + 5);
	mIndices.push_back(index + 5);
	mIndices.push_back(index + 6);
	mIndices.push_back(index + 6);
	mIndices.push_back(index + 7);
	mIndices.push_back(index + 7);
	mIndices.push_back(index + 4);

	mIndices.push_back(index + 0);
	mIndices.push_back(index + 4);
	mIndices.push_back(index + 1);
	mIndices.push_back(index + 5);
	mIndices.push_back(index + 2);
	mIndices.push_back(index + 6);
	mIndices.push_back(index + 3);
	mIndices.push_back(index + 7);
}

void LineBatch::render()
{
	mMesh->set_attribut<attribs_pack>(mBuffer.size(), mBuffer.data());
	mMesh->set_indices(mIndices.size(), mIndices.data());

	mBuffer.clear();
	mIndices.clear();
}

void LineBatch::draw()
{
	mMesh->draw(GL_LINES);
}
