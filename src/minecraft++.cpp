#include <iostream>

#include <tinyGL.h>

int main()
{
    tgl::View window(640, 480, L"Minecraft++");

    for (;window.is_open();)
    {
        tgl::event_pool();   
    }

    return 0;
}