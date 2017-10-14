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

#ifndef s2d_math_h
#define s2d_math_h

#include <math.h>
#include <float.h>
#include "s2d_macro.h"

NS_S2D

#define PI (3.1415926f)
#define FLT_EQUAL(a, b) (fabs((a) - (b)) < FLT_EPSILON)

struct vec2 {
    float x;
    float y;
};

struct vec3 {
    float x;
    float y;
    float z;
};

struct vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct size {
    float width;
    float height;
};

struct rect {
    vec2 origin;
    size size;

    static inline rect make_rect(float x, float y, float w, float h)
    {
        rect r;
        r.origin.x = x;
        r.origin.y = y;
        r.size.width = w;
        r.size.height = h;
        return r;
    }

    static inline bool contains(const rect& r, float x, float y)
    {
        return (x >= r.origin.x && x <= r.origin.x + r.size.width) &&
               (y >= r.origin.y && y <= r.origin.y + r.size.height);
    }
};

struct affine_transform {
    /*
     a b 0
     c d 0
     x y 1
     */
    float a, b, c, d;
    float x, y;

    affine_transform() {};
    affine_transform(float _a, float _b, float _c, float _d, float _x, float _y)
    : a(_a), b(_b), c(_c), d(_d), x(_x), y(_y) {}

    static affine_transform mk_identity()
    {
        return affine_transform(1, 0, 0, 1, 0, 0);
    }

    static inline affine_transform mk_translate(float x, float y)
    {
        return affine_transform(1, 0, 0, 1, x, y);
    }

    static inline affine_transform mk_scale(float x, float y)
    {
        return affine_transform(x, 0, 0, y, 0, 0);
    }

    static inline affine_transform mk_rotation(float angle)
    {
        float sin = sinf(angle * PI / 180.0f);
        float cos = cosf(angle * PI / 180.0f);

        return affine_transform(cos, sin, -sin, cos, 0, 0);
    }

    static inline vec2 apply_transform(const affine_transform& t,
                                       float x,
                                       float y)
    {
        vec2 p;
        p.x = (float)((double)t.a * x + (double)t.c * y + t.x);
        p.y = (float)((double)t.b * x + (double)t.d * y + t.y);
        return p;
    }

    static inline void inplace_concat(affine_transform& left,
                                      const affine_transform& right)
    {
        float a = left.a * right.a + left.b * right.c;
        float b = left.a * right.b + left.b * right.d;
        float c = left.c * right.a + left.d * right.c;
        float d = left.c * right.b + left.d * right.d;
        float x = left.x * right.a + left.y * right.c + right.x;
        float y = left.x * right.b + left.y * right.d + right.y;
        
        left.a = a; left.b = b; left.c = c; left.d = d; left.x = x; left.y = y;
    }

    static inline affine_transform concat(const affine_transform& left,
                                          const affine_transform& right)
    {
        float a = left.a * right.a + left.b * right.c;
        float b = left.a * right.b + left.b * right.d;
        float c = left.c * right.a + left.d * right.c;
        float d = left.c * right.b + left.d * right.d;
        float x = left.x * right.a + left.y * right.c + right.x;
        float y = left.x * right.b + left.y * right.d + right.y;
        
        return affine_transform(a, b, c, d, x, y);
    }

    static inline affine_transform invert(const affine_transform& t)
    {
        float det = t.a * t.d - t.c * t.b;
        float a =  t.d / det;
        float b = -t.b / det;
        float c = -t.c / det;
        float d =  t.a / det;
        float x = (t.c*t.y - t.d*t.x) / det;
        float y = (t.b*t.x - t.a*t.y) / det;

        return affine_transform(a, b, c, d, x, y);
    }
};

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

#endif
