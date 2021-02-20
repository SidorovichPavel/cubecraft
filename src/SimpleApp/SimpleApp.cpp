#include <src/SimpleApp/SimpleApp.h>
#include <assert.h>

std::wstring gTitle{ L"Minecraft++" };
tgl::View* App::appWindow;
tgl::Timer* App::Timer;
tgl::Shader* App::ShaderFirst;
tgl::Texture2D* App::appTexture;
Camera* App::appCamera;

tgl::TextureAtlas2D* appAtlas;

std::bitset<1024> App::appKeys;
bool App::appLockCursor = false;
float App::appMouseSensitivity = 0.025f;
float appCameraSpeed = 16.f;

size_t App::PrevTime;
float App::FrameTime;

std::vector<std::unique_ptr<voxel::Chunk>> Chunks;
voxel::Chunk* appChunk;

//timed
la::mat4 model;
la::mat4 perspective;
la::mat4 view;

std::vector<unsigned> indices{ 1,0,2,2,0,3/*,6,5,4,7,4,5,2,6,0,0,6,4,1,3,5,5,3,7,0,4,3,3,4,7,5,6,1,1,6,2 */ };

float vertices[] = {
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Верхний правый
	 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Нижний правый
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Нижний левый
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f // Верхний левый
};

//timed
float angle = 0.f;//delete this

constexpr auto gWorldWidth = 10;
constexpr auto gWorldHeight = 1;
constexpr auto gWorldDepth = 10;

void App::Init(int argn, char** argc)
{
	tgl::Init();

	appWindow = new tgl::View(640, 480, gTitle);
	appCamera = new Camera(la::vec3(0.f, 11.f, -1.f), la::vec3(0.f, 0.f, 0.f), la::vec3(0.f, 1.f, 0.f), 640.f / 480.f, 45.f);

	appWindow->enale_opengl_context();

	appWindow->size_event.attach(appCamera, &Camera::update_aspect);

	appWindow->size_event.attach([] (uint16_t x, uint16_t y)
								 {
									 tgl::gl::glViewport(0, 0, x, y);
								 });

	appWindow->mouse_wheel_event.attach(appCamera, &Camera::update_Fovy);

	appWindow->key_down_event.attach([] (int64_t code, int64_t)
									 {
										 if (code >= 0 && code < 1024)
											 appKeys[code] = true;
									 });

	appWindow->key_down_event.attach([] (int64_t code, int64_t)
									 {
										 if (appKeys[VK_TAB])
											 appLockCursor = !appLockCursor;
										 appWindow->show_cursor(!appLockCursor);
										 if (appLockCursor)
											 appWindow->enable_mouse_raw_input();
										 else
											 appWindow->disable_mouse_raw_input();
									 });

	appWindow->key_up_event.attach([] (int64_t code, int64_t)
								   {
									   if (code >= 0 && code < 1024)
										   appKeys[code] = false;
								   });

	appWindow->mouse_raw_input_event.attach([] (int32_t dx, int32_t dy)
											{
												appCamera->update_angles(dy * -appMouseSensitivity * FrameTime,
																		 dx * -appMouseSensitivity * FrameTime,
																		 0.f);
											});

	appAtlas = new tgl::TextureAtlas2D();
	std::vector<std::string> diffuse_image = {
		"res/textures/aerial_grass_rock/aerial_grass_rock_diff_1k.jpg",
		"res/textures/dirt_aerial_2/dirt_aerial_02_diff_1k.jpg",
		"res/textures/red_brick_3/red_brick_03_diff_1k.jpg",
		"res/textures/rocks_ground_2/rocks_ground_02_col_1k.jpg",
		"res/textures/rocks_ground_6/rocks_ground_06_diff_1k.jpg",
	};
	appAtlas->gen<3, 3>(diffuse_image);
	//appAtlas->bind();

	ShaderFirst = new tgl::Shader("first");
	ShaderFirst->link();

	appTexture = new tgl::Texture2D("textures/red_brick_3/red_brick_03_diff_1k.jpg");
	appTexture->bind();

	for (auto z = -gWorldDepth / 2; z <= gWorldDepth / 2; ++z)
		for (auto x = -gWorldWidth / 2; x <= gWorldWidth / 2; ++x)
		{
			std::unique_ptr<voxel::Chunk> chunk(new voxel::Chunk(x, 0, z));
			voxel::Chunk::render(chunk.get());
			Chunks.push_back(std::move(chunk));
		}



	tgl::gl::glEnable(GL_CULL_FACE);
	//tgl::gl::glCullFace(GL_BACK);
	tgl::gl::glEnable(GL_DEPTH_TEST);

	float angle = 0.f;
	PrevTime = tgl::win::GetTickCount64();
	FrameTime = 0.f;

	model = la::mat4(1.f);
	view = la::mat4(1.f);
	perspective = la::mat4(1.f);
}


void App::KeyProcessing()
{
	if (appKeys['W'] || appKeys[VK_UP])
		*appCamera += la::normalize(appCamera->get_direction()) * FrameTime * appCameraSpeed;
	if (appKeys['S'] || appKeys[VK_DOWN])
		*appCamera -= la::normalize(appCamera->get_direction()) * FrameTime * appCameraSpeed;
	if (appKeys['D'] || appKeys[VK_RIGHT])
		*appCamera += la::normalize(appCamera->get_right()) * FrameTime * appCameraSpeed;
	if (appKeys['A'] || appKeys[VK_LEFT])
		*appCamera -= la::normalize(appCamera->get_right()) * FrameTime * appCameraSpeed;
	if (appKeys[VK_SPACE])
		*appCamera += la::normalize(appCamera->get_up()) * FrameTime * appCameraSpeed;
	if (appKeys[VK_SHIFT])
		*appCamera -= la::normalize(appCamera->get_up()) * FrameTime * appCameraSpeed;

	if (appKeys['Q'] && appLockCursor)
		appCamera->update_angles(0.f, 0.f, 1.5f * FrameTime);
	if (appKeys['E'] && appLockCursor)
		appCamera->update_angles(0.f, 0.f, -1.5f * FrameTime);

	if (appKeys[VK_ESCAPE])
		appWindow->destroy();

}

void App::UpdateFrmaeTime()
{
	size_t current_time = tgl::win::GetTickCount64();
	FrameTime = (current_time - PrevTime) / 1000.f;
	PrevTime = current_time;
}

void App::Render()
{
	::App::UpdateFrmaeTime();

	::App::KeyProcessing();

	tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tgl::gl::glClearColor(0.f, 0.f, 0.f, 1.f);

	model = la::mat4(1.f);
	//model = la::rotate(model, la::vec3(1.f, 1.f, 0.f), angle);
	angle += 1.2f * FrameTime;
	view = appCamera->get_view();
	perspective = appCamera->get_perspective();

	auto transform = perspective * view * model;

	ShaderFirst->uniform_matrix4f("transform", transform.data());

	//draw section
	ShaderFirst->use();

	for (auto& e : Chunks)
		e->draw();
	//section end

	appWindow->swap_buffers();
}

int App::Run()
{
	tgl::win::MSG msg;
	size_t nextUpdate{};
	size_t fpsLock = 1000 / 60;

	while (appWindow->is_open())
	{
		size_t ms = tgl::win::GetTickCount64();
		auto msNext = nextUpdate;
		size_t wait{};
		size_t ret{ WAIT_TIMEOUT };

		while (tgl::win::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			tgl::win::TranslateMessage(&msg);
			tgl::win::DispatchMessage(&msg);
		}

		if (ms < msNext)
		{
			wait = min(fpsLock, msNext - ms);
			::App::Render();
		}

		if (wait <= 1)
		{
			nextUpdate = ms + fpsLock;
			::App::Render();
		}

		assert((wait & 0xffff0000) == 0);
		if (tgl::win::MsgWaitForMultipleObjects(0, nullptr, FALSE, static_cast<unsigned>(wait), QS_ALLEVENTS) == WAIT_TIMEOUT)
		{
			::App::Render();
			nextUpdate = tgl::win::GetTickCount64() + fpsLock;
		}
	}

	return 0;
}

void App::Terminate()
{
	delete Timer;
	delete ShaderFirst;
	delete appChunk;
	delete appTexture;
	delete appCamera;
	delete appWindow;
}
