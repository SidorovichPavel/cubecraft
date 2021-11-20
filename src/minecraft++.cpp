#include <memory>
#include <algorithm>
#include <iostream>
#include <array>
#include <chrono>

#include <src/glm/glm.hpp>
#include <src/View/View.h>
#include <src/View/Mouse.h>
#include <src/View/KeyBoard.h>
#include <src/Shader/Shader.h>
#include <src/Mesh/Mesh.h>
#include <src/tinyGL.h>
#include <src/glm/glm.hpp>
#include <src/glm/ext.hpp>
#include <src/Camera/Camera.h>
#include "Voxels/Chunks.h"

void processing(float ft, tgl::View& _View, tgl::Mouse& _Mouse, tgl::KeyBoard& _KeyBoard, ta::Camera& _Camera) noexcept;

std::pair<float, std::chrono::time_point<std::chrono::steady_clock>>
frame_time(std::chrono::time_point<std::chrono::steady_clock> _Prev_Time_Point) noexcept;

int main(int argc, char** args)
{
	tgl::Init();

#ifdef _DEBUG
	tgl::gl::glEnable(GL_DEBUG_OUTPUT);
	tgl::gl::DebugMessageCallback(tgl::gl::callback, nullptr);
#endif

	std::unique_ptr<tgl::View> window(new tgl::View(800, 600, "Cube++"));
	window->init_opengl();
	window->enale_opengl_context();

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
	mesh.gen();
	mesh.set_attribut<3, 3>(attribs.size(), attribs.data(), GL_STATIC_DRAW);
	mesh.set_indices(indices.size(), indices.data());
	mesh.bind();
	tgl::Shader::path_prefix = "res\\glsl\\";
	tgl::Shader base("base");


	auto frame = frame_time(std::chrono::steady_clock::now());
	
	std::pair<bool, int> pool_res;
	bool isRunnig = window->is_open();
	for (; isRunnig;)
	{
		pool_res = tgl::event_pool();

		if (!window->is_open())
			break;

		if (!pool_res.first)
			continue;

		tgl::clear_black();

		processing(frame.first, *window, mouse, keyboard, camera);

		base.use();
		modelMat = glm::mat4(1.f);
		auto transform = camera.get_mat4() * modelMat;
		base.uniform_matrix4f("transform", &transform);
		mesh.draw(GL_TRIANGLES);

		window->swap_buffers();

		frame = frame_time(frame.second);
	}

	return pool_res.second;
}

std::pair<float, std::chrono::time_point<std::chrono::steady_clock>>
frame_time(std::chrono::time_point<std::chrono::steady_clock> _Prev_Time_Point) noexcept
{
	auto current = std::chrono::steady_clock::now();
	std::chrono::duration<float> duration = current - _Prev_Time_Point;
	
	return std::pair<float, std::chrono::time_point<std::chrono::steady_clock>>(duration.count(), current);
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