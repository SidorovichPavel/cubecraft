#include <src/SimpleApp/SimpleApp.h>
#include <assert.h>

std::wstring gTitle{ L"Minecraft++" };
tgl::View* App::Window;
tgl::Timer* App::Timer;
tgl::Shader* App::ShaderFirst;
tgl::VAO* App::Vao;

size_t App::PrevTime;
float App::FrameTime;

//timed
la::mat4 model(1.f);
la::mat4 perspective(1.f);
la::mat4 view(1.f);

std::vector<unsigned> indices{ 0,1,2,1,0,3,6,5,4,7,4,5,2,6,0,0,6,4,1,3,5,5,3,7,0,4,3,3,4,7,5,6,1,1,6,2 };

std::vector<la::vec3> vertexes{
	{  .5f,  .5f, .5f },
	{ -.5f, -.5f, .5f },
	{  .5f, -.5f, .5f },
	{ -.5f,  .5f, .5f },
	{  .5f,  .5f, -.5f },
	{ -.5f, -.5f, -.5f },
	{  .5f, -.5f, -.5f },
	{ -.5f,  .5f, -.5f },
};

std::vector<la::vec3> colors{
	{1.f, 0.f, 0.f},
	{1.f, 0.f, 0.f},
	{1.f, 0.f, 0.f},
	{1.f, 0.f, 0.f},
	{0.f, 0.f, 1.f},
	{0.f, 0.f, 1.f},
	{0.f, 0.f, 1.f},
	{0.f, 0.f, 1.f},
};

void App::Init(int argn, char** argc)
{
	tgl::Init();

	Window = new tgl::View(640, 480, gTitle);
	Window->enale_opengl_context();
	Window->size_event.attach([] (uint16_t x, uint16_t y) -> void
							  {
								  tgl::gl::glViewport(0, 0, x, y);
								  perspective = la::perspeñtive(0.1f, 100.f,
																static_cast<float>(x) / static_cast<float>(y),
																45.f);
							  });
	perspective = la::perspeñtive(0.1f, 100.f, 640.f / 480.f, 45.f);

	Vao = new tgl::VAO();
	Vao->add_vertex_buffer(vertexes.data(), vertexes.size() * la::vec3::count());
	Vao->add_vertex_buffer(colors.data(), vertexes.size() * la::vec3::count());
	Vao->add_indices_buffer(indices.data(), indices.size());

	ShaderFirst = new tgl::Shader("first");
	ShaderFirst->bind_attribute(0, "position");
	ShaderFirst->bind_attribute(1, "color");
	ShaderFirst->link();

	tgl::gl::glEnable(GL_CULL_FACE);
	tgl::gl::glEnable(GL_DEPTH_TEST);

	float angle = 0.f;
	PrevTime = tgl::win::GetTickCount64();
	FrameTime = 0.f;

	Timer = new tgl::Timer(Window->get_handle(), 200);
}

//timed
float angle = 0.f;//delete this

void App::Render()
{
	tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tgl::gl::glClearColor(0.f, 0.f, 0.f, 1.f);

	model = la::mat4(1.f);
	model = la::rotate(model,
					   la::vec3(std::cosf(angle) * std::sinf(angle),
								std::tan(angle),
								std::coshf(angle)),
					   angle);
	angle += 0.5f * FrameTime;
	view = la::lock_at(la::vec3(0.f, 2.f, 1.f),
					   la::vec3(0.f, 0.f, 0.f),
					   la::vec3(0.f, 1.f, 0.f));

	auto transform = perspective * view * model;

	ShaderFirst->uniform_matrix4f("transform", transform.data());

	//draw section
	ShaderFirst->use();
	Vao->draw(GL_TRIANGLES);

	//section end

	Window->swap_buffers();

	if (Timer->check())
	{
		std::wstring out = gTitle;
		out += L"; fps : ";
		out += std::to_wstring(1.f / FrameTime);
		out += L"ms;";
		Window->set_title(out);

		Timer->reset();
	}
}

int App::Run()
{
	tgl::win::MSG msg;
	size_t nextUpdate{};
	size_t fpsLock = 1000 / 60;

	while (Window->is_open())
	{
		if (tgl::win::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			tgl::win::TranslateMessage(&msg);
			tgl::win::DispatchMessage(&msg);
		}

		size_t ms = tgl::win::GetTickCount64();
		auto msNext = nextUpdate;
		size_t wait{};
		size_t ret{ WAIT_TIMEOUT };

		if (ms < msNext)
		{
			wait = min(fpsLock, msNext - ms);
			::App::Render();
			FrameTime = (tgl::win::GetTickCount64() - ms) / 1000.f;
		}

		if (wait <= 1)
		{
			nextUpdate = ms + fpsLock;
			::App::Render();
			FrameTime = (tgl::win::GetTickCount64() - ms) / 1000.f;
		}

		assert((wait & 0xffff0000) == 0);
		if (tgl::win::MsgWaitForMultipleObjects(0, nullptr, FALSE, static_cast<unsigned>(wait), QS_ALLEVENTS) == WAIT_TIMEOUT)
		{
			::App::Render();
			size_t current_time = tgl::win::GetTickCount64();
			nextUpdate = current_time + fpsLock;
			FrameTime = (current_time - ms) / 1000.f;
		}
	}

	return 0;
}

void App::Terminate()
{
	delete Timer;
	delete ShaderFirst;
	delete Vao;
	delete Window;
}
