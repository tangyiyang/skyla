/*
 * Renderer Interface Class.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_renderer_h
#define s2d_renderer_h

#include "s2d_common.h"

NS_S2D

typedef uint16_t index_t;
struct uv_t {
    uint16_t u;
    uint16_t v;
};

struct pos_tex_color_vertex {
    vec2 pos;
    uv_t uv;
    color_t color;
};

struct pos_color_vertex {
    vec2 pos;
    color_t color;
};

enum blend_mode {
    BLEND_MODE_NONE    = -1,
    BLEND_MODE_NORMAL  = 0,
    BLEND_MODE_ADDTIVE = 1,
    BLEND_MODE_MUTIPLY = 2,
    BLEND_MODE_SCREEN  = 3,
    BLEND_MODE_ALPHA   = 4,
    BLEND_MODE_MAX
};

struct blend_func {
    GLenum src;
    GLenum dst;

    static blend_func NORMAL;
    static blend_func ADDITIVE;
    static blend_func MULTIPLY;
    static blend_func SCREEN;
    static blend_func ALPHA;
};

class renderer {
public:
    virtual ~renderer() {};
    virtual void init()     = 0;
    virtual void shutdown() = 0;
    virtual void flush()    = 0;

    static inline blend_func blend_mode_to_func(blend_mode mode)
    {
        static const blend_func blend_funcs[BLEND_MODE_MAX] = {
            blend_func::NORMAL,
            blend_func::ADDITIVE,
            blend_func::MULTIPLY,
            blend_func::SCREEN,
            blend_func::ALPHA
        };
        return blend_funcs[mode];
    }

};


NS_S2D_END

#endif
