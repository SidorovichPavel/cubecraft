#pragma once

#include <string>
#include <src/tinyGL.h>
#include <src/linearAlgebra.h>

class ClientWindow final :
    public tgl::View
{
    float mFov;
public:
    ClientWindow(const int width, const int height, const std::wstring& title, tgl::Style style = tgl::Style());
    la::mat4 get_perspective();
    void set_title(const std::wstring& title);
private:
    void size(int width, int height);
    void mouse_move(int x, int y, __int64 virtual_code);
    void moving(tgl::win::RECT* pRect);
};

