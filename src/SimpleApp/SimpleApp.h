#pragma once

#include <bitset>
#include <src/incs.h>

class App
{
private:
	static voxel::Chunks* appChunks;

	static tgl::View* appWindow;
	static tgl::Timer* Timer;
	static tgl::Shader* ShaderFirst;
	static tgl::Texture2D* appTexture;
	static Camera* appCamera;

	static std::bitset<1024> appKeys;
	static bool appLockCursor;
	static float appMouseSensitivity;

	static size_t PrevTime;
	static float FrameTime;

	static void BindEvents();
	static void DetachEvents();
	static void Render();
	static void UpdateFrmaeTime();
	static void KeyProcessing();
public:
	static void Init(int argn, char** argc);
	static int Run();
	static void Terminate();
};