//
//  Created by yiyang.tang on 2017/9/13.
//

#ifndef s2d_math_h
#define s2d_math_h

#include <math.h>
#include "s2d_macro.h"

NS_S2D

#define PI (3.1415926f)

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

struct affine_transform {
    float a, b, c, d;
    float x, y;

    affine_transform() {};
    affine_transform(float _a, float _b, float _c, float _d, float _x, float _y)
    : a(_a), b(_b), c(_c), d(_d), x(_x), y(_y) {}

    static affine_transform identity;
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

    static inline affine_transform mk_rotation(float x, float y)
    {
    float sin = sinf(x * PI / 180.0f);
    float cos = cosf(y * PI / 180.0f);

    return affine_transform(cos, -sin, sin, cos, 0, 0);
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

    static inline void inplace_concat(affine_transform& t1,
                                      const affine_transform& t2)
    {
    float a = t1.a * t2.a + t1.b * t2.c;
    float b = t1.a * t2.b + t1.b * t2.d;
    float x = t1.x * t2.a + t1.y * t2.c + t2.x;

    float c = t1.c * t2.a + t1.d * t2.c;
    float d = t1.c * t2.b + t1.d * t2.d;
    float y = t1.x * t2.b + t1.y * t2.d + t2.y;

    t1.a = a; t1.b = b; t1.x = x;
    t1.c = c; t1.d = d; t1.y = y;
    }

    static inline affine_transform concat(const affine_transform& t1,
                                          const affine_transform& t2)
    {
    float a = t1.a * t2.a + t1.b * t2.c;
    float b = t1.a * t2.b + t1.b * t2.d;
    float x = t1.x * t2.a + t1.y * t2.c + t2.x;

    float c = t1.c * t2.a + t1.d * t2.c;
    float d = t1.c * t2.b + t1.d * t2.d;
    float y = t1.x * t2.b + t1.y * t2.d + t2.y;

    return affine_transform(a, b, c, d, x, y);
    }
};
NS_S2D_END

#endif /* s2d_math_h */
