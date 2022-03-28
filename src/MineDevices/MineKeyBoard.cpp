#include "MineKeyBoard.hpp"

namespace tgl::win
{
	#include <Windows.h>
}

MineKeyBoard::MineKeyBoard()
	:
	base()
{
}

MineKeyBoard::~MineKeyBoard() noexcept
{
}

bool MineKeyBoard::is_default() const noexcept
{
	return mKeyStates[VK_ESCAPE];
}

bool MineKeyBoard::is_move_front() const noexcept
{
	return mKeyStates['W'];
}

bool MineKeyBoard::is_move_back() const noexcept
{
	return mKeyStates['S'];
}

bool MineKeyBoard::is_move_left() const noexcept
{
	return mKeyStates['A'];
}

bool MineKeyBoard::is_move_right() const noexcept
{
	return mKeyStates['D'];
}
