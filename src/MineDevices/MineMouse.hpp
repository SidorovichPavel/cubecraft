#pragma once

#include <View/Mouse.hpp>

class MineMouse final :
    public tgl::Mouse
{
    using base = tgl::Mouse;

public:
    MineMouse(int32_t _X = 0, int32_t _Y = 0);
    ~MineMouse() noexcept;
};

