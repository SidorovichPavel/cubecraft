#include "Camera.h"
#include <src/glm/ext.hpp>
#include <src/glm/matrix.hpp>

Camera::Camera(const glm::vec3 _Pos, const glm::vec3 _Target, glm::vec3 _Up, const float _Aspect, const float _Fovy)
	:
	mPosition(_Pos),
	mDirection(glm::normalize(_Target - _Pos)),
	mUp(_Up),
	mRight(mDirection* mUp),
	mAspect(_Aspect),
	mFovy(_Fovy),
	mRotate(glm::mat4(1.f)),
	mPitch(0.f),
	mYaw(0.f),
	mRoll(0.f),
	useQuaternions(false)
{
	//mYaw = acosf(mDirection[0]);
	//mPitch = acosf(mDirection[1]);
}

Camera::~Camera()
{}

Camera& Camera::operator+=(const glm::vec3& _Vec)noexcept
{
	mPosition += _Vec;
	return *this;
}

Camera& Camera::operator-=(const glm::vec3& _Vec)noexcept
{
	mPosition -= _Vec;
	return *this;
}

glm::mat4 Camera::get_perspective() const noexcept
{
	return glm::perspective(mFovy, mAspect, 0.1f, 300.f);
}

glm::mat4 Camera::get_view() const noexcept
{
	return glm::lookAt(mPosition,
					   mPosition + mDirection,
					   mUp);
}

glm::mat4 Camera::get_mat4() const noexcept
{
	return get_perspective() * get_view();
}

glm::vec3 Camera::get_direction() const noexcept
{
	return mDirection;
}

glm::vec3 Camera::get_right() const noexcept
{
	return mRight;
}

glm::vec3 Camera::get_up() const noexcept
{
	return mUp;
}

glm::vec3 Camera::get_position() const noexcept
{
	return mPosition;
}

void Camera::update_Fovy(uint16_t _Keys, int16_t _Delta, int32_t x, int32_t y) noexcept
{
	if (_Delta > 0)
		if ((mFovy - 44.5f) > std::numeric_limits<float>::epsilon())
			mFovy -= 0.1f;
	if (_Delta < 0)
		if ((45.5f - mFovy) > std::numeric_limits<float>::epsilon())
			mFovy += 0.1f;
}

void Camera::update_aspect(uint16_t x, uint16_t y)noexcept
{
	if (y != 0)
		mAspect = static_cast<float>(x) / static_cast<float>(y);
	else
		mAspect = 0.01f;
}

void Camera::update_vertors() noexcept
{
	mDirection = glm::vec3(mRotate * glm::vec4({ 0.f, 0.f, -1.f, 1.f }));
	mRight = glm::vec3(mRotate * glm::vec4({ 1.f, 0.f, 0.f, 1.f }));
	mUp = glm::vec3(mRotate * glm::vec4({ 0.f, 1.f, 0.f, 1.f }));
}

//angle pls in radians
void Camera::update_angles(float _Pitch, float _Yaw, float _Roll)
{
	mPitch += _Pitch; mYaw += _Yaw; mRoll += _Roll;
	mRotate = glm::mat4(1.f);

	mRotate = glm::rotate(mRotate, mRoll, glm::vec3({ 0.f, 0.f, 1.f }));
	mRotate = glm::rotate(mRotate, mYaw, glm::vec3({ 0.f, 1.f, 0.f }));
	mRotate = glm::rotate(mRotate, mPitch, glm::vec3({ 1.f, 0.f, 0.f }));
	update_vertors();
}

void Camera::use_quaternoins(bool _mode)
{
	useQuaternions = _mode;
}
