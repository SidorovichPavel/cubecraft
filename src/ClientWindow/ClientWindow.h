#pragma once

#include <string>
#include <src/tinyGL.h>

class ClientWindow final :
    public tgl::View
{
public:
    ClientWindow(const int width, const int height, const std::wstring& title, tgl::Style style = tgl::Style());
private:
    void size(int width, int height);
};

