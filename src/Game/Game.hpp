#pragma once

#include <stdint.h>
#include <array>
#include <vector>
#include <memory>

#include <Camera/Camera.hpp>

#include <Logger/Logger.hpp>
#include <Utility/utility.hpp>
#include <View/View.hpp>
#include <Mesh/Mesh.hpp>
#include <Shader/FileShader.hpp>

#include "..\MineDevices\MineMouse.hpp"
#include "..\MineDevices\MineKeyBoard.hpp"

namespace game
{

	class Menu //
	{
	public:
		enum class Section
		{
			load,
			start,
			settings,
			adv_settings
		};


		Menu();
		~Menu();


	public:
		
		Section mSection;
	};

	class Scene //view
	{

	};

	class GameState // controller
	{
		glm::vec3 mWorldSize;

		MineMouse mMouse;
		MineKeyBoard mKeyboard;

		tgl::View* mView;
		tgl::Event<void>::id_type
			mMouseShiftEventID,
			mKeyPressEventID,
			mKeyReleaseEventID[2],
			mEventSizeCameraID,
			mEventMouseWheleCameraID,
			mEventSetFocusID,
			mEventKillFocusID;

		ta::Camera mCamera;

		tgl::Mesh mSphereMesh;
		std::unique_ptr<tgl::Shader> mShaderPtr;

		tgl::Mesh mCubesMesh;
		std::unique_ptr<tgl::Shader> mCubesShader;

		bool mIsShowCursor;
		uint64_t mWorldTime;
	public:
		GameState(tgl::View * _View);
		~GameState();

		void render() noexcept;
		void update(int64_t _FrameTime) noexcept;

	private:
		void init();

		void on_release_button(uint64_t _Code, int64_t _State) noexcept;
		void on_set_focus() noexcept;
		void on_kill_focus() noexcept;
	};


}