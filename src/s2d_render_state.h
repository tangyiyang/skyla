/*
 * Render State Management Class
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_render_state_h
#define s2d_render_state_h

#include "s2d_renderer.h"
#include "s2d_texture.h"
NS_S2D

class sprite;
class particle;


class render_state {
public:
    enum render_type {
        RENDERER_TYPE_QUAD = 0,
        MAX_RENDERER_TYPE,
    };

public:
    void init();
    void shutdown();
    void clear();
    void flush();

    void draw_quad(const affine_transform& t, texture* tex, blend_mode b, pos_tex_color_vertex* p);
    void draw_sprite(sprite* s);
    void draw_particle(particle* p);

    void push_scissors(const rect& r);
    void pop_scissors();

private:
    renderer* switch_renderer(render_type type);

private:
    render_type  _cur_renderer_type;
    renderer* _cur_renderer;
    renderer* _renderers[MAX_RENDERER_TYPE];
    std::stack<rect> _scissors_stack;
    blend_mode _mode;
};

NS_S2D_END

#endif
