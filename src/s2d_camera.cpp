/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

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
