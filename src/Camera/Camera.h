#pragma once

#include <src/linearAlgebra.h>

class Camera
{
	la::vec3 mPosition;
	la::vec3 mDirection;
	la::vec3 mUp;
	la::vec3 mRight;
	float mAspect{};
	float mFovy{};

	la::mat4 mRotate;

	float mPitch;
	float mYaw;
	float mRoll;

	bool useQuaternions;

	void update_vertors() noexcept;
public:
	Camera(const la::vec3 _Pos, const la::vec3 _Targe, la::vec3 _Up, const float _Aspect, const float _Fovy);
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	~Camera();

	Camera& operator+=(const la::vec3& _Vec)noexcept;
	Camera& operator-=(const la::vec3& _Vec)noexcept;

	la::mat4 get_perspective() const noexcept;
	la::mat4 get_view()const noexcept;
	la::vec3 get_direction() const noexcept;
	la::vec3 get_right() const noexcept;
	la::vec3 get_up() const noexcept;
	la::vec3 get_position() const noexcept;

	void update_Fovy(uint16_t _Keys, int16_t _Delta, int32_t x, int32_t y) noexcept;
	void update_aspect(uint16_t x, uint16_t y)noexcept;
	void update_angles(float x, float y, float z);
	void use_quaternoins(bool _mode = true);
};

