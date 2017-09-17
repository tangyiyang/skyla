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

#endif /* s2d_math_h */
