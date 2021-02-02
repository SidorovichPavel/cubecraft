#include <iostream>

#include <src/tinyGL.h>

int main()
{
    tgl::Init();

    tgl::View window(640, 480, L"Minecraft++");
    
    std::array<float, 9> triangle{
        0.f, .5f, 0.f,
        -.5f, -.5f, 0.f,
        .5f, -.5f, 0.f
    };

    tgl::VAO vao;
    vao.push_vbo(triangle);

    tgl::Shader first("first");
    first.bind_attribute(0, "position");
    first.link();

    for (;window.is_open();)
    {
        tgl::event_pool();
        tgl::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tgl::gl::glClearColor(0.75f, 0.5f, 0.35f, 1.f);
        
        vao.draw(3);

        window.swap_buffers();
    }

    return 0;
}