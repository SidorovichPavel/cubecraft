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
	static ClientWindow* Window;
	static tgl::Timer* Timer;
	static tgl::Shader* ShaderFirst;
	static tgl::VAO* Vao;

	static size_t PrevTime;
	static float FrameTime;
	static void Render();
public:
	static void Init(int argn, char** argc);
	static int Run();
	static void Terminate();
};