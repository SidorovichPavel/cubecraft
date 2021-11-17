#include <memory>
#include <algorithm>
#include <iostream>
#include <array>

#include <src/glm/glm.hpp>
#include <src/View/View.h>
#include <src/View/Mouse.h>
#include <src/Shader/Shader.h>
#include <src/Mesh/Mesh.h>
#include <src/tinyGL.h>


int main(int argc, char** args)
{
	tgl::Init();

	std::unique_ptr<tgl::View> window(new tgl::View(800, 600, "Cube++"));
	window->init_opengl();
	window->enale_opengl_context();

	tgl::Mouse mouse;
	auto& events = window->events();
	events.mouse_raw_input.attach(&mouse, &tgl::Mouse::raw_input);
	events.size.attach([](int16_t _X, int16_t _Y)
		{
			tgl::gl::glViewport(0, 0, _X, _Y);
		});
	std::array<float, 18> attribs =
	{
		-1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 1.f, 0.f,
		1.f, -1.f, 0.f, 0.f, 0.f, 1.f
	};
	std::array<unsigned, 3> indices = { 0,1,2 };

	tgl::Mesh mesh;
	mesh.set_attribut<3,3>(attribs.size(), attribs.data());
	mesh.set_indices(indices.size(), indices.data());
	mesh.bind();
	tgl::Shader base("base");

	std::pair<bool, int> pool_res;
	bool isRunnig = window->is_open();
	for (;isRunnig;)
	{
		pool_res = tgl::event_pool();

		if (!window->is_open())
			break;
		
		if (!pool_res.first)
			continue;

		

		tgl::clear_black();
		
		base.use();
		mesh.draw(GL_TRIANGLES);

		window->swap_buffers();
	}

	return pool_res.second;
}