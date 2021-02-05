#include "ClientWindow.h"

ClientWindow::ClientWindow(const int width, const int height, const std::wstring& title, tgl::Style style):
	View(width, height, title, style)
{}

void ClientWindow::size(int width, int height)
{
	tgl::gl::glViewport(0, 0, width, height);
}
