/*
 * Render State Management Class
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_render_state_h
#define skyla_render_state_h

#include "renderer.h"
#include "renderer/texture.h"
NS_SKYLA

class sprite;
class scale9sprite;
class particle;
class primitive_node;

class render_state {
public:
    enum render_type {
        RENDERER_TYPE_QUAD = 0,
        RENDERER_TYPE_LINE,
        MAX_RENDERER_TYPE,
    };

public:
    void init();
    void shutdown();
    void clear();
    void flush();

    void draw_quad(const affine_transform& t, texture* tex, blend_mode b, pos_tex_color_vertex* p);
    void draw_sprite(sprite* s);
    void draw_scale9sprite(scale9sprite* s);
    void draw_particle(particle* p);
    void draw_primitive(primitive_node* p);

    void push_scissors(const rect& r);
    void pop_scissors();

private:
    renderer* switch_renderer(render_type type);

private:
    render_type      _cur_renderer_type;
    renderer*        _cur_renderer;
    renderer*        _renderers[MAX_RENDERER_TYPE];
    std::stack<rect> _scissors_stack;
    blend_mode       _mode;
};

NS_SKYLA_END

#endif
