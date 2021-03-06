/*
 * General Geometry types and math operations,
 * TODO: we should implement the Matrix operations in SSE instructions.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_math_h
#define skyla_math_h

#include <math.h>
#include <float.h>
#include "skyla_macro.h"

NS_SKYLA

#define FLT_EQUAL(a, b) (fabs((a) - (b)) < FLT_EPSILON)

#define SKYLA_TEX_COORD_MAX (USHRT_MAX)

#define uniform_to_uint16(u) (uint16_t)((u)*((float)(SKYLA_TEX_COORD_MAX)))
#define make_gl_color(color) ((color >> 24) & 0x000000ff) | ((color >> 8) & 0x0000ff00) | ((color << 8) & 0x00ff0000) | ((color << 24) & 0xff000000)

#define COLOR4F_TO_UINT32(r, g, b, a) make_gl_color(((((int)(r*255))<<24) + (((int)(g*255))<<16) + (((int)(b*255))<<8) + ((int)a*255)))

typedef uint32_t color_t;

#define COLOR_WHITE (0xffffffff)
#define COLOR_RED   (0xff0000ff)
#define COLOR_GREEN (0x00ff00ff)
#define COLOR_BLUE  (0x0000ffff)

struct vec2 {
    float x;
    float y;

    static inline vec2 make(float x, float y)
    {
        struct vec2 v;
        v.x = x;
        v.y = y;
        return v;
    }

    static inline void normalize(float x, float y, vec2& out)
    {
        float n = x*x + y*y;
        if (n < FLT_EPSILON) {
            out.x = 0.0f;
            out.y = 0.0f;
        } else {
            float len = sqrtf(n);
            out.x = x / len;
            out.y = y / len;
        }
    }

    static inline void normalize(const vec2& v, vec2& out)
    {
        float n = v.x * v.x + v.y *v.y;
        if (n < FLT_EPSILON) {
            out.x = 0.0f;
            out.y = 0.0f;
        } else {
            float len = sqrtf(n);
            out.x = v.x/len;
            out.y = v.y/len;
        }
    }
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

struct color4f {
    float r;
    float g;
    float b;
    float a;
};

struct size {
    float width;
    float height;

    static inline size make(float width, float height)
    {
        return {width, height};
    }
};

class math {
public:
    static float clampf(float v, float min, float max)
    {
        const float t = v < min ? min : v;
        return t > max ? max : t;
    }
};

struct border {
    float left;
    float right;
    float bottom;
    float top;

    static inline border make(float left, float right, float bottom, float top)
    {
        border b;
        b.left = left;
        b.right = right;
        b.bottom = bottom;
        b.top = top;
        return b;
    }
};

struct rect {
    vec2 origin;
    size size;

    static inline rect make(float x, float y, float w, float h)
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

    static inline rect merge(const rect& l, const rect& r)
    {
        float min_x = std::min(l.origin.x, r.origin.x);
        float max_x = std::max(l.origin.x + l.size.width, r.origin.x + r.size.width);
        float min_y = std::min(l.origin.y, r.origin.y);
        float max_y = std::max(l.origin.y + l.size.height, r.origin.y + r.size.height);

        return {
            .origin.x = min_x,
            .origin.y = min_y,
            .size.width = max_x - min_x,
            .size.height = max_y - min_y
        };
    }

    inline std::string to_string()
    {
        char buf[64];
        snprintf(buf, 32, "{x = %.2f, y = %.2f, width = %.2f, height = %.2f}",
                 origin.x, origin.y, size.width, size.height);
        return std::string(buf);
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
        float sin = sinf(angle * M_PI / 180.0f);
        float cos = cosf(angle * M_PI / 180.0f);

        return affine_transform(cos, sin, -sin, cos, 0, 0);
    }

    static inline vec2 apply_transform(const affine_transform& t,
                                       float x,
                                       float y)
    {
        vec2 p;
        p.x = t.a * x + t.c * y + t.x;
        p.y = t.b * x + t.d * y + t.y;
        return p;
    }

    static inline rect apply_transform(const affine_transform& t,
                                       const rect& _rect)
    {
        float left  = _rect.origin.x;
        float right = _rect.origin.x + _rect.size.width;
        float bottom = _rect.origin.y;
        float top = _rect.origin.y + _rect.size.height;

        vec2 tl = apply_transform(t, left, top);
        vec2 tr = apply_transform(t, right, top);
        vec2 bl = apply_transform(t, left, bottom);
        vec2 br = apply_transform(t, right, bottom);

        float min_x = std::min(std::min(tl.x, tr.x), std::min(bl.x, br.x));
        float max_x = std::max(std::max(tl.x, tr.x), std::max(bl.x, br.x));
        float min_y = std::min(std::min(tl.y, tr.y), std::min(bl.y, br.y));
        float max_y = std::max(std::max(tl.y, tr.y), std::max(tl.y, br.y));

        return rect::make(min_x, min_y, (max_x - min_x), (max_y - min_y));
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
        vec2 v = {
            ml->m[0] * v2->x + ml->m[3] * v2->y + ml->m[6] * 0.0f,
            ml->m[1] * v2->x + ml->m[4] * v2->y + ml->m[7] * 0.0f
        };
        return v;
    }
};
NS_SKYLA_END

#endif
