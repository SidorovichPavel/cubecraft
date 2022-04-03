#include <memory>
#include <algorithm>
#include <iostream>
#include <array>
#include <chrono>
#include <functional>

#include <tinyGL.hpp>

#include "Game/Game.hpp"

int main(int argc, char** args)
{
	tgl::Init();

#ifdef _DEBUG
	tgl::gl::glEnable(GL_DEBUG_OUTPUT);
	tgl::gl::DebugMessageCallback(tgl::gl::callback, nullptr);
#endif
	
	auto style = new tgl::Style("Cube++");
	std::unique_ptr<tgl::View> window(new tgl::View(style));
	window->init_opengl();
	window->enable_opengl_context();
	auto minecraft = std::make_unique<game::GameState>(*window);
	minecraft->init();

	auto& events = window->get_events();
	events.size.attach(tgl::view_port);
	
	tgl::detail::FrameTimeInfo ft_info;
	bool isRunnig = window->is_open();
	for (; isRunnig;)
	{
		auto [update, state] = tgl::event_pool(30, ft_info);
		isRunnig = window->is_open();

		if (!update && !window->is_open())
			continue;

		tgl::clear_black();

		minecraft->update(ft_info.ms());
		minecraft->render();

		window->swap_buffers();

		ft_info.update_frame_time();
	}

	return 0;
}