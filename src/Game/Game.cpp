#include "Game.hpp"

#include <tinyGL.hpp>
#include <MarchingCubes/MarchingCubes.hpp>

#include <glm\ext\scalar_constants.hpp>

namespace game
{
	GameState::GameState(tgl::View& _View) noexcept
		:
		mView(&_View),
		mWorldSize({ 10,10,10 }),
		mCamera(
			glm::vec3(0.f, 0.f, 2.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			_View.get_ratio(),
			45.f),
		mIsShowCursor(true)
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

	float circle(const glm::vec3& v)
	{
		return length(v) - 2.f;
	}

	void GameState::init() throw()
	{
		tgl::gl::glEnable(GL_DEPTH_TEST);
		tgl::gl::glEnable(GL_CULL_FACE);

		std::array<float, 12> attribs =
		{
			-1.f,  1.f, 0.f,
			-1.f, -1.f, 0.f,
			 1.f, -1.f, 0.f,
			 1.f,  1.f, 0.f,
		};
		std::array<float, 12> attrib_color =
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f,
			1.f, 0.f, 0.f,
		};
		std::array<unsigned, 6> indices = { 0,1,2,2,3,0 };

		mMesh.set_attribut(0, 3, attribs.size(), attribs.data(), tgl::GlDrawMode::Static);
		mMesh.set_attribut(1, 3, attrib_color.size(), attrib_color.data(), tgl::GlDrawMode::Static);
		mMesh.set_indices(indices.size(), indices.data(), tgl::GlDrawMode::Static);

		auto push_vtcs = [](tgl::Mesh& _Mesh, size_t _Idx, size_t _NumberOfElementPerVertex, std::vector<glm::vec3>& _Vec, tgl::GlDrawMode _DrawMode)
		{
			_Mesh.set_attribut(_Idx, _NumberOfElementPerVertex, _Vec.size() * 3, &_Vec.data()->x, _DrawMode);
		};

		auto [vtcs, idcs] = ta::marching_cubes::generate_cubes(glm::vec3(-1.3f), glm::vec3(2.5f), .2f);
		push_vtcs(mCubesMesh, 0, 3, vtcs, tgl::GlDrawMode::Static);
		mCubesMesh.set_indices(idcs.size(), idcs.data(), tgl::GlDrawMode::Static);

		auto circle = [](glm::vec3 _Pos, float _R, float _AngleStep)
		{
			uint32_t idx_num = 0;
			std::vector<glm::vec3> vertices;
			std::vector<uint32_t> indices;

			constexpr float pi2 = 2.f * glm::pi<float>();

			for (float o = 0.f; o < pi2; o += _AngleStep)
				for (float f = 0.f; f < pi2; f += _AngleStep)
				{
					glm::vec3 p(
						std::sinf(o) * std::cosf(f),
						std::sinf(o) * std::sinf(f),
						std::cosf(o));
					vertices.push_back(p * _R + _Pos);
					indices.push_back(idx_num++);
				}
			return std::make_pair(std::move(vertices), std::move(indices));
		};

		auto [circle_vtcs, circle_idcs] = circle(glm::vec3(0.f, 0.f, 0.f), 1.f, 0.023f);
		push_vtcs(mCircleMesh, 0, 3, circle_vtcs, tgl::GlDrawMode::Static);
		mCircleMesh.set_indices(circle_idcs.size(), circle_idcs.data());

		auto [fill_circle_vtcs, fill_circle_normals, fill_circle_idcs] = 
			ta::marching_cubes::march_cubes(glm::vec3(-3.f), glm::vec3(6.f), 0.25f, 0.05f, ::game::circle);
		push_vtcs(mFillCircleMesh, 0, 3, fill_circle_vtcs, tgl::GlDrawMode::Static);
		push_vtcs(mFillCircleMesh, 1, 3, fill_circle_normals, tgl::GlDrawMode::Static);
		mFillCircleMesh.set_indices(fill_circle_idcs.size(), fill_circle_idcs.data());

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
		mFillCircleMesh.draw(GL_TRIANGLES);

		mCubesShader->use();
		mCubesShader->uniform_matrix4f("camera_mat", &world);
		//mCubesMesh.draw(GL_LINES);

		//mCircleMesh.draw(GL_POINTS);

	}

	void GameState::update(float _FrameTime) noexcept
	{
		static float angle = 0.f;
		angle += 0.25 * _FrameTime;
 		glm::vec3 light_pos(std::cosf(angle), 0.f, std::sinf(angle));
		light_pos *= 3.f;
		mShaderPtr->use();
		mShaderPtr->uniform_vector3f("uLightPos", &light_pos);

		if (mKeyboard.is_move_front())
			mCamera += mCamera.get_direction() * _FrameTime;
		if (mKeyboard.is_move_back())
			mCamera -= mCamera.get_direction() * _FrameTime;
		if (mKeyboard.is_move_right())
			mCamera += mCamera.get_right() * _FrameTime;
		if (mKeyboard.is_move_left())
			mCamera -= mCamera.get_right() * _FrameTime;
		if (mKeyboard.is_default())
			mCamera.set_fovy(45.f);

		if (mMouse.get_update_state())
		{
			auto [dx, dy] = mMouse.get_shift();
			mCamera.update_angles(-glm::radians(dy * _FrameTime), -glm::radians(dx * _FrameTime), 0.f);
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