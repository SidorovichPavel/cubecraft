#include "Game.hpp"

#include <tinyGL.hpp>
#include <MarchingCubes/MarchingCubes.hpp>

#include <glm\ext\scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Octree\Octree.hpp>

namespace game
{
	GameState::GameState(tgl::View* _View)
		:
		mView(_View),
		mWorldSize({ 10,10,10 }),
		mCamera(
			glm::vec3(0.f, 0.f, 10.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			_View->get_ratio(),
			60.f),
		mIsShowCursor(true),
		mWorldTime(0)
	{
		decltype(auto) events = mView->get_events();
		mMouseShiftEventID = events.mouse_shift.attach(&mMouse, &MineMouse::shift);

		mKeyPressEventID = events.key_down.attach(&mKeyboard, &MineKeyBoard::key_down);
		mKeyReleaseEventID[0] = events.key_up.attach(this, &GameState::on_release_button);
		mKeyReleaseEventID[1] = events.key_up.attach(&mKeyboard, &MineKeyBoard::key_up);

		mEventSizeCameraID = events.size.attach(&mCamera, &ta::Camera::update_aspect);
		mEventMouseWheleCameraID = events.mouse_wheel.attach(&mCamera, &ta::Camera::update_Fovy);

		mEventSetFocusID = events.set_focus.attach(this, &GameState::on_set_focus);
		mEventKillFocusID = events.kill_focus.attach(this, &GameState::on_kill_focus);

		init();
	}

	GameState::~GameState()
	{
		decltype(auto) events = mView->get_events();
		events.mouse_shift.detach(mMouseShiftEventID);
		events.key_down.detach(mKeyPressEventID);
		events.key_up.detach(mKeyReleaseEventID[0]);
		events.key_up.detach(mKeyReleaseEventID[1]);

		events.size.detach(mEventSizeCameraID);
		events.mouse_wheel.detach(mEventMouseWheleCameraID);

		events.set_focus.detach(mEventSetFocusID);
		events.kill_focus.detach(mEventKillFocusID);
	}

	void GameState::init()
	{
		tgl::gl::glEnable(GL_DEPTH_TEST);
		tgl::gl::glEnable(GL_CULL_FACE);

		// af3 = array of 3 float
		auto set_vertieces_from_af3 = [](tgl::Mesh& _Mesh, size_t _Idx, size_t _NumberOfElementPerVertex, std::vector<glm::vec3>& _Vec, tgl::GlDrawMode _DrawMode)
		{
			_Mesh.set_attribut(_Idx, _NumberOfElementPerVertex, _Vec.size() * 3, &_Vec.data()->x, _DrawMode);
		};

		auto [vtcs, idcs] = ta::marching_cubes::generate_cubes(glm::vec3(-1.3f), glm::vec3(2.5f), .2f);
		set_vertieces_from_af3(mCubesMesh, 0, 3, vtcs, tgl::GlDrawMode::Static);
		mCubesMesh.set_indices(idcs.size(), idcs.data(), tgl::GlDrawMode::Static);

		auto tree = ta::GeneratingOctree(glm::vec3(0.f), glm::vec3(5.f), [](glm::vec3 p) {return length(p) - 2.f; });
		tree.generate(glm::vec3(0.5f));

		auto [oct_vtcs, oct_normals, oct_idcs] =
			ta::marching_cubes::march_cubes(tree);
		set_vertieces_from_af3(mSphereMesh, 0, 3, oct_vtcs, tgl::GlDrawMode::Static);
		set_vertieces_from_af3(mSphereMesh, 1, 3, oct_normals, tgl::GlDrawMode::Static);
		mSphereMesh.set_indices(oct_idcs.size(), oct_idcs.data());

		tgl::Shader::path_prefix = "res\\glsl\\";
		try
		{
			mShaderPtr = std::make_unique<tgl::FileShader>("base");
			mCubesShader = std::make_unique<tgl::FileShader>("lines");
		}
		catch (const std::runtime_error& re)
		{
			tgl::logger << re.what() << tgl::flush;
		}
		mShaderPtr->use();
		glm::vec3 objectColor(.85f, .65f, .25f);
		mShaderPtr->uniform_vector3f("uObjectColor", &objectColor);
		glm::vec3 lightColor(1.f);
		mShaderPtr->uniform_vector3f("uLightColor", &lightColor);

		mCubesShader->use();
		glm::vec3 lc(1.f, 1.f, 0.f);
		mCubesShader->uniform_vector3f("uColor", &lc);
	}

	void GameState::render() noexcept
	{
		auto world = mCamera.get_mat4(0.1f, 100.f);
		glm::mat4 model(1.f);

		mShaderPtr->use();
		mShaderPtr->uniform_matrix4f("world", &world);
		mShaderPtr->uniform_matrix4f("model", &model);
		mSphereMesh.draw(GL_TRIANGLES);

		mCubesShader->use();
		mCubesShader->uniform_matrix4f("camera_mat", &world);
		//mCubesMesh.draw(GL_LINES);
	}

	void GameState::update(int64_t _FrameTime) noexcept
	{
		if (_FrameTime == 0)
			return;

		mWorldTime += _FrameTime;
		auto delta_time = _FrameTime / 1000.f;

		static float theta = 0.f, phi = 0.f;
		glm::vec3 light_pos;
		light_pos.x = sin(theta) * cos(phi);
		light_pos.y = sin(theta) * sin(phi);
		light_pos.z = cos(theta);

		light_pos *= 10.f;
		mShaderPtr->use();
		mShaderPtr->uniform_vector3f("uLightPos", &light_pos);

		glm::vec3 shift(0.f);

		using tgl::KeyCode;
		bool make_move = false;
		if (mKeyboard[KeyCode::W])
			shift += (make_move = true, mCamera.get_direction());
		if (mKeyboard[KeyCode::S])
			shift -= (make_move = true, mCamera.get_direction());
		if (mKeyboard[KeyCode::D])
			shift += (make_move = true, mCamera.get_right());
		if (mKeyboard[KeyCode::A])
			shift -= (make_move = true, mCamera.get_right());

		if (make_move)
			mCamera += normalize(shift) * delta_time;

		if (mKeyboard.is_default())
			mCamera.set_fovy(45.f);

		phi += (mKeyboard[KeyCode::UP]) ? 1.5f * delta_time : 0.f;
		phi -= (mKeyboard[KeyCode::DOWN]) ? 1.5f * delta_time : 0.f;

		theta += (mKeyboard[KeyCode::RIGHT]) ? 1.5f * delta_time : 0.f;
		theta -= (mKeyboard[KeyCode::LEFT]) ? 1.5f * delta_time : 0.f;

		if (mMouse.get_update_state())
		{
			auto [dx, dy] = mMouse.get_shift();
			mCamera.update_angles(-glm::radians(dy * delta_time), -glm::radians(dx * delta_time), 0.f);
		}
	}


	void GameState::on_release_button(uint64_t _Code, int64_t _State) noexcept
	{
		auto is_release = [this, _Code](tgl::KeyCode _TargetCode)
		{
			return _Code == static_cast<uint32_t>(_TargetCode) && mKeyboard[_TargetCode]; 
		};

		if (is_release(tgl::KeyCode::TAB))
		{
			mIsShowCursor = !mIsShowCursor;
			mView->show_cursor(mIsShowCursor);
		}
	}

	void GameState::on_set_focus() noexcept
	{
		if (!mIsShowCursor)
			mView->show_cursor(false);
	}

	void GameState::on_kill_focus() noexcept
	{
		if (!mIsShowCursor)
			mView->show_cursor(true);
	}

}