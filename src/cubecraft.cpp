#include <memory>
#include <algorithm>
#include <iostream>
#include <array>
#include <chrono>
#include <functional>

#include <tinyGL.hpp>

#include "Game/Game.hpp"

int wmain(int argc, const wchar_t* args[])
{
	tgl::Init();

#ifdef _DEBUG
	tgl::gl::glEnable(GL_DEBUG_OUTPUT);
	tgl::gl::DebugMessageCallback(tgl::gl::callback, nullptr);

	auto fps = 30;
#else
	auto fps = 60;
#endif
	
	auto style = new tgl::Style(args[0]);
	auto window = std::make_unique<tgl::View>(style);

	window->init_opengl();
	window->enable_opengl_context();

	auto& events = window->get_events();
	auto event_size_id = events.size.attach(tgl::view_port);

	auto cubecraft = std::make_unique<game::GameState>(window.get());
	
	tgl::detail::FrameTimeInfo ft_info;
	bool isRunnig = window->is_open();
	for (; isRunnig;)
	{
		auto [update, state] = tgl::event_pool(fps, ft_info);
		isRunnig = window->is_open();

		if (!window->is_open())
			continue;

		tgl::clear_black();

		cubecraft->update(ft_info.ms());
		cubecraft->render();

		window->swap_buffers();

		ft_info.update_frame_time();
	}

	events.size.detach(event_size_id);

	return 0;
}