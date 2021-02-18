#pragma once

#include <functional>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <mutex>

#include <src/incs.h>

class App
{
private:
	static tgl::View* appWindow;
	static tgl::Timer* Timer;
	static tgl::Shader* ShaderFirst;
	static tgl::VAO* Vao;
	static Camera* appCamera;

	static bool appKeys[1024];
	static bool appLockCursor;
	static float appMouseSensitivity;

	static size_t PrevTime;
	static float FrameTime;
	static void UpdateFrmaeTime();
	static void Render();
	static void KeyProcessing();
public:
	static void Init(int argn, char** argc);
	static int Run();
	static void Terminate();
};