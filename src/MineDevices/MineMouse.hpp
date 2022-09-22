#pragma once

#include <View/Mouse.hpp>

class MineMouse final :
    public tgl::Mouse
{
    using base = tgl::Mouse;

    float mSensitivity;

public:
    MineMouse(int32_t _X = 0, int32_t _Y = 0);
    ~MineMouse() noexcept;

    void shift(int32_t _LastX, int32_t _LastY) noexcept override;
};

