#include "Game.hpp"

#include <tinyGL.hpp>
#include <MarchingCubes/MarchingCubes.hpp>

#include <glm\ext\scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Octree\Octree.hpp>

namespace game
{
	GameState::GameState(tgl::View& _View) noexcept
		:
		mView(&_View),
		mWorldSize({ 10,10,10 }),
		mCamera(
			glm::vec3(0.f, 0.f, 10.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			_View.get_ratio(),
			60.f),
		mIsShowCursor(true),
		mWorldTime(0)
	{
		decltype(auto) events = mView->get_events();
		mMouseRawInputEventID = events.mouse_shift.attach(&mMouse, &MineMouse::shift);

		mKeyPressEventID = events.key_down.attach(&mKeyboard, &MineKeyBoard::key_down);
		mKeyReleaseEventID[0] = events.key_up.attach(this, &GameState::on_release_button);
		mKeyReleaseEventID[1] = events.key_up.attach(&mKeyboard, &MineKeyBoard::key_up);

		mEventSizeCameraID = events.size.attach(&mCamera, &ta::Camera::update_aspect);
		mEventMouseWheleCameraID = events.mouse_wheel.attach(&mCamera, &ta::Camera::update_Fovy);

		events.set_focus.attach(this, &GameState::on_set_focus);
		events.kill_focus.attach(this, &GameState::on_kill_focus);
	}

	GameState::~GameState()
	{


		decltype(auto) events = mView->get_events();
		events.mouse_shift.detach(mMouseRawInputEventID);
		events.key_down.detach(mKeyPressEventID);
		events.key_up.detach(mKeyReleaseEventID[0]);
		events.key_up.detach(mKeyReleaseEventID[1]);

		events.size.detach(mEventSizeCameraID);
		events.mouse_wheel.detach(mEventMouseWheleCameraID);


	}

	float circle(glm::vec3 v)
	{
		return length(v) - 2.f;
	}

	void GameState::init() throw()
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

		auto tree = ta::Octree(glm::vec3(0.f), glm::vec3(5.f));
		tree.generate(glm::vec3(0.5f), ::game::circle);

		auto [oct_vtcs, oct_normals, oct_idcs] =
			ta::marching_cubes::march_cubes(tree, ::game::circle);
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

	void GameState::update(uint64_t _FrameTime) noexcept
	{
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

		if (mKeyboard.is_move_front())
			mCamera += mCamera.get_direction() * delta_time;
		if (mKeyboard.is_move_back())
			mCamera -= mCamera.get_direction() * delta_time;
		if (mKeyboard.is_move_right())
			mCamera += mCamera.get_right() * delta_time;
		if (mKeyboard.is_move_left())
			mCamera -= mCamera.get_right() * delta_time;
		if (mKeyboard.is_default())
			mCamera.set_fovy(45.f);
		
		phi += (mKeyboard[VK_UP]) ? 1.5f * delta_time : 0.f;
		phi -= (mKeyboard[VK_DOWN]) ? 1.5f * delta_time : 0.f;
		
		theta += (mKeyboard[VK_RIGHT]) ? 1.5f * delta_time : 0.f;
		theta -= (mKeyboard[VK_LEFT]) ? 1.5f * delta_time : 0.f;

		if (mMouse.get_update_state())
		{
			auto [dx, dy] = mMouse.get_shift();
			mCamera.update_angles(-glm::radians(dy * delta_time), -glm::radians(dx * delta_time), 0.f);
		}
	}


	void GameState::on_release_button(uint64_t _Code, int64_t _State) noexcept
	{
		if (_Code == VK_TAB && mKeyboard[VK_TAB])
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