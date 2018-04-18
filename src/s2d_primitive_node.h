/*
 * render lines, rect angles, polygons.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_primitive_node_h
#define s2d_primitive_node_h

#include "s2d_node.h"

NS_S2D

struct line {
    line(const vec2& _begin, const vec2& _end, color_t _color) : begin(_begin), end(_end), color(_color) {}

    vec2 begin;
    vec2 end;
    color_t color;
};

class primitive_node : public node {
public:
    primitive_node() {}
    virtual ~primitive_node() {}

public:
    void draw(render_state* rs) override;

public:
    void add_line(const vec2& begin, const vec2& end, color_t color);
    void add_rect(const vec2& bottom_left, const size& size, color_t color);

public:
    std::vector<line> _lines;
};

NS_S2D_END

#endif
