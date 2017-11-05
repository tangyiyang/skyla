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

class renderer {
public:
    virtual ~renderer() {};
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void flush() = 0;
};


NS_S2D_END

#endif
