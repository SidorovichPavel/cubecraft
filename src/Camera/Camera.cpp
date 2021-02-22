#include "Camera.h"
#include <corecrt_math.h>
#include <math.h>

Camera::Camera(const la::vec3 _Pos, const la::vec3 _Target, la::vec3 _Up, const float _Aspect, const float _Fovy)
	:
	mPosition(_Pos),
	mDirection(la::normalize(_Target - _Pos)),
	mUp(_Up),
	mRight(mDirection* mUp),
	mAspect(_Aspect),
	mFovy(_Fovy),
	mRotate(la::mat4(1.f)),
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

Camera& Camera::operator+=(const la::vec3& _Vec)noexcept
{
	mPosition += _Vec;
	return *this;
}

Camera& Camera::operator-=(const la::vec3& _Vec)noexcept
{
	mPosition -= _Vec;
	return *this;
}

la::mat4 Camera::get_perspective() const noexcept
{
	return la::perspeñtive(0.1f, 300.f, mAspect, mFovy);
}

la::mat4 Camera::get_view() const noexcept
{
	return la::lock_at(mPosition,
					   mPosition + mDirection,
					   mUp);
}

la::vec3 Camera::get_direction() const noexcept
{
	return mDirection;
}

la::vec3 Camera::get_right() const noexcept
{
	return mRight;
}

la::vec3 Camera::get_up() const noexcept
{
	return mUp;
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
	mDirection = la::vec3(mRotate * la::vec4({ 0.f, 0.f, -1.f, 1.f }));
	mRight = la::vec3(mRotate * la::vec4({ 1.f, 0.f, 0.f, 1.f }));
	mUp = la::vec3(mRotate * la::vec4({ 0.f, 1.f, 0.f, 1.f }));
}

//angle pls in radians
void Camera::update_angles(float _Pitch, float _Yaw, float _Roll)
{
	mPitch += _Pitch; mYaw += _Yaw; mRoll += _Roll;

	if (useQuaternions)
	{
		la::Quaternion qx(mRight, _Pitch);
		la::Quaternion qy(mUp, _Yaw);
		la::Quaternion qz(mDirection, -_Roll);

		auto q = qx * qy * qz;
		mDirection = mDirection * q;
		mRight = mRight * q;
		mUp = mUp * q;
	}
	else
	{
		mRotate = la::mat4(1.f);
		mRotate = la::rotate(mRotate, la::vec3({ 0.f, 0.f, 1.f }), mRoll);
		mRotate = la::rotate(mRotate, la::vec3({ 0.f, 1.f, 0.f }), mYaw);
		mRotate = la::rotate(mRotate, la::vec3({ 1.f, 0.f, 0.f }), mPitch);
		update_vertors();
	}
}

void Camera::use_quaternoins(bool _mode)
{
	useQuaternions = _mode;
}
