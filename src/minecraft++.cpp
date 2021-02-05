#include <iostream>
#include <vector>
#include <array>

#include <src/tinyGL.h>
#include <src/linearAlgebra.h>
#include <src/ClientWindow/ClientWindow.h>

int main()
{
	tgl::Init();

	ClientWindow window(640, 480, L"Minecraft++");

	std::vector<la::vec3> vertexes {
		{0.f, .5f, 0.f },
		{-.5f, -.5f, 0.f},
		{.5f,-.5f, 0.f}
	};

	std::vector<la::vec3> colors{
		{1.f, 0.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 0.f, 1.f}
	};

	tgl::VAO vao;
	vao.add_vertex_buffer_object(vertexes.data(), vertexes.size() * la::vec3::count());
	vao.add_vertex_buffer_object(colors.data(), vertexes.size() * la::vec3::count());

	tgl::Shader first("first");
	first.bind_attribute(0, "position");
	first.bind_attribute(1, "color");
	first.link();

	first.use();

	for (; window.is_open();)
	{
		tgl::event_pool();

		vao.draw(3);

		window.swap_buffers();
	}

	return 0;
}