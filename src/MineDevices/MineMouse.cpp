#include "MineMouse.hpp"

MineMouse::MineMouse(int32_t _X, int32_t _Y)
	:
	base(_X, _Y),
	mSensitivity(6.f)
{
}

MineMouse::~MineMouse() noexcept
{
}

void MineMouse::shift(int32_t _LastX, int32_t _LastY) noexcept
{
	mUpdated = true;

	lastX = x;
	lastY = y;

	x += static_cast<int32_t>(_LastX * mSensitivity);
	y += static_cast<int32_t>(_LastY * mSensitivity);
}
