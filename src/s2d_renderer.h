/*
 * Renderer Interface Class.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_renderer_h
#define s2d_renderer_h

#include "s2d_common.h"
#include "s2d_texture.h"
#include "s2d_util.h"

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

    inline static void assign(pos_tex_color_vertex* vert, float x, float y, uint16_t u, uint16_t v, color_t color)
    {
        vert->pos.x = x;
        vert->pos.y = y;
        vert->uv.u = u;
        vert->uv.v = v;
        vert->color = color;
    }
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

class texture_blend_protocol {
public:

    void init()
    {
        _blend_mode = BLEND_MODE_NORMAL;
        _texture = nullptr;
    }

    inline void set_blend_mode(blend_mode mode)
    {
        _blend_mode = mode;
    }

    inline blend_mode get_blend_mode()
    {
        return _blend_mode;
    }

    inline void set_texture(texture* tex)
    {
        S2D_ASSERT(tex);

        _texture = tex;
        _blend_mode = _texture->_premultiply_alpha ? BLEND_MODE_NORMAL : BLEND_MODE_ALPHA;
    }

public:
    blend_mode _blend_mode;
    texture*   _texture;
};


NS_S2D_END

#endif
