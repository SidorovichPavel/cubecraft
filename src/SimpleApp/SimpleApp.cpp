#include <src/SimpleApp/SimpleApp.h>
#include <assert.h>
#include <iostream>

std::wstring gTitle{ L"Minecraft++" };
tgl::View* App::appWindow;
tgl::Shader* App::ShaderFirst;
tgl::Shader* App::MineShader;
tgl::Shader* App::LinesShader;
tgl::Mesh* App::WhiteCube;
tgl::Texture2D* App::appTexture;
tgl::Texture2D* App::appTexture2;
tgl::Texture2D* appTexture3;
LineBatch* App::appLineBatch;
Camera* App::appCamera;

tgl::TextureAtlas2D* appAtlas;

std::bitset<1024> App::appKeys;
bool App::appLockCursor = false;
float App::appMouseSensitivity = 0.025f;
float appCameraSpeed = 16.f;

size_t App::PrevTime;
float App::FrameTime;

voxel::Chunks* App::appChunks;
bool rb_press = false;

//timed
la::mat4 model;
la::mat4 perspective;
la::mat4 view;

struct Cube
{
	int32_t
		x0, x1,
		y0, y1,
		z0, z1;
};

constexpr Cube gWorldCube{ -1,1,0,1,-1,1 };
constexpr auto gWorldWidth = gWorldCube.x1 - gWorldCube.x0;
constexpr auto gWorldHeight = gWorldCube.y1 - gWorldCube.y0;
constexpr auto gWorldDepth = gWorldCube.z1 - gWorldCube.z0;

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




void App::BindEvents()
{
	if (!appWindow || !appCamera)
		throw std::runtime_error("what you doing");

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
										 if (code == VK_TAB)
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
	appWindow->mouse_rbutton_up.attach([] (int64_t, int32_t x, int32_t)
									   {
										   rb_press = false;
									   });
	appWindow->mouse_rbutton_down.attach([] (int64_t, int32_t x, int32_t)
										 {
											 rb_press = true;
										 });
}

void App::DetachEvents()
{
	assert(appWindow);
	appWindow->disable_mouse_raw_input();
	appWindow->create_event.detach_all();
	appWindow->key_down_event.detach_all();
	appWindow->key_up_event.detach_all();
	appWindow->mouse_move_event.detach_all();
	appWindow->mouse_raw_input_event.detach_all();
	appWindow->mouse_wheel_event.detach_all();
	appWindow->move_event.detach_all();
	appWindow->moving_event.detach_all();
	appWindow->raw_input_event.detach_all();
	appWindow->size_event.detach_all();
}

void App::Init(int argn, char** argc)
{
	tgl::Init();

	appWindow = new tgl::View(640, 480, gTitle);
	appWindow->init_opengl();
	appWindow->enale_opengl_context();
	appCamera = new Camera(la::vec3({ 0.f, 50.f, 1.f }), la::vec3({ 0.f, 0.f, 0.f }), la::vec3({ 0.f, 0.f, -1.f }), 640.f / 480.f, 45.f);
	//appCamera->use_quaternoins();

	::App::BindEvents();

	#ifdef _DEBUG
	tgl::gl::glEnable(GL_DEBUG_OUTPUT);
	tgl::gl::DebugMessageCallback(tgl::callback, nullptr);
	#endif

	MineShader = new tgl::Shader("mine");
	MineShader->link();

	MineShader->use();
	MineShader->uniform_int("material.diffuse", 0);
	tgl::gl::ActiveTexture(GL_TEXTURE0);
	appTexture = new tgl::Texture2D("textures/red_brick_3/red_brick_03_diff_1k.jpg");
	appTexture->bind();

	MineShader->uniform_int("material.normal", 1);
	tgl::gl::ActiveTexture(GL_TEXTURE1);
	appTexture2 = new tgl::Texture2D("textures/red_brick_3/red_brick_03_nor_1k.jpg");
	appTexture2->bind();
	MineShader->uniform_int("material.specular", 2);
	tgl::gl::ActiveTexture(GL_TEXTURE3);
	appTexture3 = new tgl::Texture2D("textures/red_brick_3/red_brick_03_spec_1k.jpg");
	appTexture3->bind();

	ShaderFirst = new tgl::Shader("white");
	ShaderFirst->link();

	WhiteCube = new tgl::Mesh();
	WhiteCube->set_attribut<3>(vertexes.size() * 3, (float*)vertexes.data());
	WhiteCube->set_indices(indices.size(), indices.data());

	LinesShader = new tgl::Shader("lines");
	LinesShader->link();

	appLineBatch = new LineBatch(50);
	appLineBatch->line(la::vec3{ 0.f,0.f,0.f }, la::vec3{ 0.f,20.f,0.f }, la::vec4{ 1.f,0.f,0.f,1.f });
	appLineBatch->box(la::vec3{ 0.f,20.f,0.f }, la::vec3{ 1.2f,1.2f,1.2f }, la::vec4{ 1.f,0.f,0.f,1.f });
	appLineBatch->render();

	appChunks = new voxel::Chunks(gWorldWidth, gWorldHeight, gWorldDepth);

	for (auto y = gWorldCube.y0; y < gWorldCube.y1; ++y)
		for (auto z = gWorldCube.z0; z < gWorldCube.z1; ++z)
			for (auto x = gWorldCube.x0; x < gWorldCube.x1; ++x)
				appChunks->emplace_back(x, y, z);
	appChunks->set_neightbors(gWorldWidth, gWorldHeight, gWorldDepth);



	tgl::gl::glEnable(GL_CULL_FACE);
	//tgl::gl::glCullFace(GL_BACK);
	tgl::gl::glEnable(GL_DEPTH_TEST);

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

	if (appKeys['Q'] && appLockCursor && false)
		appCamera->update_angles(0.f, 0.f, 1.5f * FrameTime);
	if (appKeys['E'] && appLockCursor && false)
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

	{
		la::vec3 end, norm, iend;
		if (rb_press)
		{
			auto vox = appChunks->ray_cast(appCamera->get_position(), la::normalize(appCamera->get_direction()),
								   8.f,
								   end, norm, iend);
			if (vox)
				appChunks->set((int)iend[0] + (int)norm[0], (int)iend[1] + (int)norm[1], (int)iend[2] + (int)norm[2], 1);
		}

	}


	//render section
	for (auto& chunk : *appChunks)
		voxel::Chunk::render(chunk);
	//render

	//shader-draw section
	{
		model = la::mat4(1.f);
		view = appCamera->get_view();
		perspective = appCamera->get_perspective();
		auto transform = perspective * view * model;


		static float angle = 0.f;
		la::vec3 light_pos = la::vec3{
			20 * cosf(angle),
			20 * sinf(angle),
			0.f
		};
		angle += 0.015f;
		la::vec3 light_pos2{ 0,25,0 };

		tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tgl::gl::glClearColor(0.f, 0.f, 0.f, 1.f);

		MineShader->use();
		MineShader->uniform_matrix4f("transform", transform.data());
		MineShader->uniform_vector3f("light_pos", light_pos2.data());//light_pos.data());//appCamera->get_position().data());
		MineShader->uniform_vector3f("light_color", la::vec3(1.f).data());

		for (auto& e : *appChunks)
			e.draw();

		model = la::translate(model, light_pos2);

		transform = perspective * view * model;
		ShaderFirst->use();
		ShaderFirst->uniform_matrix4f("transform", transform.data());
		WhiteCube->draw(GL_TRIANGLES);

		LinesShader->use();
		LinesShader->uniform_matrix4f("camera_mat4", (perspective * view).data());
		appLineBatch->draw();
	}
	//draw end

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
	delete ShaderFirst;
	delete MineShader;
	delete appChunks;
	delete appTexture;
	delete appTexture2;
	delete appTexture3;
	delete appLineBatch;
	delete appAtlas;
	delete appCamera;
	delete appWindow;
}
