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
    vec2 begin;
    vec2 end;
    color_t color;
};

class primitive_node : public node {
public:
    primitive_node() {}
    virtual ~primitive_node() {}

public:
    void init() override;
    bool update(float dt) override;
    void draw(render_state* rs) override;

public:
    void draw_line(const vec2& begin, const vec2& end, color_t color);
    void draw_rect(const vec2& bottom_left, const vec2& top_right, color_t color);

};

NS_S2D_END

#endif
