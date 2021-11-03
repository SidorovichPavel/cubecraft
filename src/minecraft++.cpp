

#include <memory>
#include <algorithm>


#include <src/glm/glm.hpp>
#include <src/tinyGL.h>
#include <src/View/View.h>
#undef min max

#include <mutex>


int main(int argn, char** argc)
{
	tgl::Init();

	using view_t = tgl::View<tgl::win::WinHandler>;

	std::unique_ptr<view_t> window(new view_t(800, 600, L"Cube++"));
	window->init_opengl();
	window->enale_opengl_context();

	std::pair<bool, int> pool_res;
	bool isRunnig = window->is_open();
	for (;isRunnig;)
	{
		pool_res = tgl::event_pool();

		if (!pool_res.first)
			continue;

		if (!window->is_open())
			break;

		

		tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tgl::gl::glClearColor(.65f, .35f, .25f, 1.f);

		window->swap_buffers();
	}

	return pool_res.second;
}