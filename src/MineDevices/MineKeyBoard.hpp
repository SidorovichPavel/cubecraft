#pragma once

#include <View\KeyBoard.hpp>

class MineKeyBoard final :
	public tgl::KeyBoard
{
	using base = tgl::KeyBoard;

public:
	MineKeyBoard();
	~MineKeyBoard() noexcept;

	bool is_default() const noexcept;

	bool is_move_front() const noexcept;
	bool is_move_back() const noexcept;
	bool is_move_left() const noexcept;
	bool is_move_right() const noexcept;
};