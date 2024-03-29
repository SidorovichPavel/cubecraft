#pragma once

#include <bitset>
#include <map>
#include <string>

class App
{
private:
	enum Shader
	{
		ShMain = 0,
		ShLine,
		ShScreenCross,

		ShCount,
	};


	static voxel::Chunks* appChunks;

	static tgl::View* appWindow;
	static tgl::Timer* Timer;
	static std::vector<tgl::Shader*> appShaders;
	static tgl::Shader* ShaderFirst;
	static tgl::Mesh* WhiteCube;
	static tgl::Mesh* ScreenCross;
	static tgl::Texture2D* appTexture;
	static tgl::Texture2D* appTexture2;
	static LineBatch* appLineBatch;
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