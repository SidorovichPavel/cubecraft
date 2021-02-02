#include <iostream>

#include <tinyGL.h>

#include <gl/GL.h>

int main()
{
    tgl::View window(640, 480, L"Minecraft++");

    for (;window.is_open();)
    {
        tgl::event_pool();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.65, 0.4, 0.15, 1);
        window.swap_buffers();
    }

    return 0;
}