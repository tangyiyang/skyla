//
//  Created by yiyang.tang on 2017/9/13.
//

#include "s2d_camera.h"

NS_S2D

void camera::init_orthographic(float width, float height)
{
    float* m = _matrix.m;

    float left = 0;
    float right = width;
    float bottom = 0;
    float top = height;
    m[0] = 2 / (right - left);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 2 / (top - bottom);
    m[5] = 0;
    m[6] = (left + right) / (left - right);
    m[7] = (top + bottom) / (bottom - top);
    m[8] = 1;

    _size = { width, height };
}

void camera::translate(const vec2& offset)
{
}

NS_S2D_END
