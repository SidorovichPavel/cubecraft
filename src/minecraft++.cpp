#include <memory>
#include <algorithm>
#include <iostream>
#include <array>
#include <chrono>

#include <src/Style/Style.hpp>
#include <src/View/View.hpp>
#include <src/View/Mouse.hpp>
#include <src/View/KeyBoard.hpp>
#include <src/Shader/Shader.hpp>
#include <src/Mesh/Mesh.hpp>
#include <src/tinyGL.hpp>

#include <src/glm/glm.hpp>
#include <src/glm/ext.hpp>
#include <src/Camera/Camera.h>

#include "Utils/Utility.hpp"
#include "Voxels/Chunks.hpp"

void processing(float ft, tgl::View& _View, tgl::Mouse& _Mouse, tgl::KeyBoard& _KeyBoard, ta::Camera& _Camera) noexcept;

int main(int argc, char** args)
{
	tgl::Init();

#ifdef _DEBUG
	tgl::gl::glEnable(GL_DEBUG_OUTPUT);
	tgl::gl::DebugMessageCallback(tgl::gl::callback, nullptr);
#endif

	auto style = new tgl::Style("Cube++", 0, 0, 640, 480);
	std::unique_ptr<tgl::View> window(new tgl::View(style));
	window->init_opengl();
	window->enable_opengl_context();
	
	tgl::Mouse mouse;
	tgl::KeyBoard keyboard;
	auto& events = window->events();
	events.mouse_raw_input.attach(&mouse, &tgl::Mouse::raw_input);
	events.key_down.attach(&keyboard, &tgl::KeyBoard::key_down);
	events.key_up.attach(&keyboard, &tgl::KeyBoard::key_up);
	events.size.attach(std::bind(tgl::view_port_ex, 0, 0, std::placeholders::_1, std::placeholders::_2));
	
	ta::Camera camera(
		glm::vec3(0.f, 0.f, 2.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		window->get_ratio(),
		45.f);

	events.size.attach(&camera, &ta::Camera::update_aspect);
	events.mouse_wheel.attach(&camera, &ta::Camera::update_Fovy);

	glm::mat4 modelMat(1.f);

	std::array<float, 18> attribs =
	{
		-1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 1.f, 0.f,
		1.f, -1.f, 0.f, 0.f, 0.f, 1.f
	};
	std::array<unsigned, 3> indices = { 0,1,2 };

	tgl::Mesh mesh;
	mesh.set_attribut<3, 3>(attribs.size(), attribs.data(), GL_STATIC_DRAW);
	mesh.set_indices(indices.size(), indices.data());
	mesh.bind();
	tgl::Shader::path_prefix = "res\\glsl\\";
	tgl::Shader base("base");

	auto update_frame_time = [](std::chrono::time_point<std::chrono::steady_clock> _Prev_TP)
	{
		auto current = std::chrono::steady_clock::now();
		std::chrono::duration<float> duration = current - _Prev_TP;

		return FrameTimeInfo(duration.count(), current);
	};
	FrameTimeInfo frame = update_frame_time(std::chrono::steady_clock::now());

	bool isRunnig = window->is_open();

	for (; isRunnig;)
	{
		auto [update, wParam] = tgl::event_pool();
		
		isRunnig = window->is_open();

		if (!update || !window->is_open())
			continue;

		tgl::clear_black();

		processing(frame.duration, *window, mouse, keyboard, camera);

		base.use();
		modelMat = glm::mat4(1.f);
		auto transform = camera.get_mat4() * modelMat;
		base.uniform_matrix4f("transform", &transform);
		mesh.draw(GL_TRIANGLES);

		window->swap_buffers();

		frame = update_frame_time(frame.timepoint);
	}

	return 0;
}

void processing(float ft, tgl::View& _View, tgl::Mouse& _Mouse, tgl::KeyBoard& _KeyBoard, ta::Camera& _Camera) noexcept
{
	if (_KeyBoard.is_move_front())
		_Camera += _Camera.get_direction() * ft;
	if (_KeyBoard.is_move_back())
		_Camera -= _Camera.get_direction() * ft;
	if (_KeyBoard.is_move_right())
		_Camera += _Camera.get_right() * ft;
	if (_KeyBoard.is_move_left())
		_Camera -= _Camera.get_right() * ft;
	if (_KeyBoard.is_default())
		_Camera.set_fovy(45.f);
}