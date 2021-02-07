#include <iostream>
#include <vector>
#include <array>

#include <src/tinyGL.h>
#include <src/linearAlgebra.h>
#include <src/ClientWindow/ClientWindow.h>

int main()
{
	tgl::Init();

	std::wstring title{ L"Minecraft++" };
	ClientWindow window(640, 480, title);
	window.enale_opengl_context();

	std::vector<la::vec3> vertexes{
		{-1.f, 0.f, 0.f },
		{-1.f, -1.f, 0.f },
		{0.f, 0.f, 0.f },
		{0.f, -1.f, 0.f }
	};

	std::vector<la::vec3> colors{
		{1.f, 0.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f},
	};

	std::vector<unsigned> indices{ 0, 1, 2, 2, 1, 3 };

	tgl::VAO vao;
	vao.add_vertex_buffer(vertexes.data(), vertexes.size() * la::vec3::count());
	vao.add_vertex_buffer(colors.data(), vertexes.size() * la::vec3::count());
	vao.add_indices_buffer(indices.data(), indices.size());

	tgl::Shader first("first");
	first.bind_attribute(0, "position");
	first.bind_attribute(1, "color");
	first.link();

	first.use();

	float angle = 0.f;

	auto prev_time = tgl::win::GetTickCount64();
	auto current_time = prev_time;
	float frame_time = 0.f;

	tgl::Timer timer(window.get_handle(), 500);

	tgl::gl::glEnable(GL_CULL_FACE);

	return tgl::App::Run([&]
		{
			tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			tgl::gl::glClearColor(0.f, 0.f, 0.f, 1.f);

			la::mat4 model(1.f);//model = la::scale(model, la::vec3(0.4f, 0.4f, 1.f));
			la::mat4 view(1.f);
			la::mat4 perspective(1.f);
			model = la::translate(model, la::vec3(0.f, 0.f, -2.f));
			model = la::rotate(model, la::vec3(0.f, 1.f, 0.f), angle); angle += 0.5f * frame_time;
			first.uniform_matrix4f("model", model.data());

			first.uniform_matrix4f("view", view.data());

			perspective = window.get_perspective();
			first.uniform_matrix4f("perspective", perspective.data());

			vao.draw(GL_TRIANGLES);

			tgl::event_pool();
			window.swap_buffers();

			current_time = tgl::win::GetTickCount64();
			frame_time = (current_time - prev_time) / 1000.f;
			prev_time = current_time;

			if (timer.check())
			{
				std::wstring out = title;
				out += L"; fps : ";
				out += std::to_wstring(1.f / frame_time);
				out += L"ms;";
				window.set_title(out);

				timer.reset();
			}
		});
}