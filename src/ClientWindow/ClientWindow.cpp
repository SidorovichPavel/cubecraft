#include "ClientWindow.h"

ClientWindow::ClientWindow(const int width, const int height, const std::wstring& title, tgl::Style style):
	View(width, height, title, style),
	mFov(45.f)
{}

la::mat4 ClientWindow::get_perspective()
{
	return la::perspeñtive(0.1f, 100.f, static_cast<float>(mWidth) / static_cast<float>(mHeight), mFov);
}

void ClientWindow::set_title(const std::wstring& title)
{
	tgl::win::SetWindowText(this->mHandle, title.c_str());
}

void ClientWindow::size(int width, int height)
{
	mWidth = width;
	mHeight = height;

	tgl::gl::glViewport(0, 0, mWidth, mHeight);
}

void ClientWindow::mouse_move(int x, int y, __int64 virtual_code)
{
	tgl::win::InvalidateRect(mHandle, NULL, NULL);
}

void ClientWindow::moving(tgl::win::RECT* pRect)
{
	tgl::win::InvalidateRect(mHandle, nullptr, FALSE);
}

