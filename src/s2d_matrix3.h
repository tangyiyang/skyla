/****************************************************************************
 Copyright (c) Yiyang Tang

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef s2d_matrix3_h
#define s2d_matrix3_h

#include "s2d_common.h"

NS_S2D

struct matrix3
{
    float m[9];

    inline static void set(matrix3* m, float* array)
    {
        memcpy(m->m, array, 9 * sizeof(float));
    }

    inline static matrix3 multiply(matrix3* l, matrix3* r)
    {
        matrix3 m;

        m.m[0] = l->m[0] * r->m[0] + l->m[3] * r->m[1] + l->m[6] * r->m[2];
        m.m[3] = l->m[0] * r->m[3] + l->m[3] * r->m[4] + l->m[6] * r->m[5];
        m.m[6] = l->m[0] * r->m[6] + l->m[3] * r->m[7] + l->m[6] * r->m[8];

        m.m[1] = l->m[1] * r->m[0] + l->m[4] * r->m[1] + l->m[7] * r->m[2];
        m.m[4] = l->m[1] * r->m[3] + l->m[4] * r->m[4] + l->m[7] * r->m[5];
        m.m[7] = l->m[1] * r->m[6] + l->m[4] * r->m[7] + l->m[7] * r->m[8];

        m.m[2] = l->m[2] * r->m[0] + l->m[5] * r->m[1] + l->m[8] * r->m[2];
        m.m[5] = l->m[2] * r->m[3] + l->m[5] * r->m[4] + l->m[8] * r->m[5];
        m.m[8] = l->m[2] * r->m[6] + l->m[5] * r->m[7] + l->m[8] * r->m[8];

        return m;
    }

    inline static vec2 multiply_vec2(matrix3* ml, const vec2* v2)
    {
        vec2 v = { ml->m[0] * v2->x + ml->m[3] * v2->y + ml->m[6] * 0.0f,
                   ml->m[1] * v2->x + ml->m[4] * v2->y + ml->m[7] * 0.0f };
        return v;
    }
};

NS_S2D_END

#endif /* s2d_matrix3_h */
