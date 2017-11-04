/*
 * Render State Management Class
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_render_state_h
#define s2d_render_state_h

#include "s2d_renderer.h"

NS_S2D

class sprite;
class render_state {
public:
    enum RENDERER_TYPE {
        RENDERER_TYPE_QUAD = 0,
        MAX_RENDERER_TYPE,
    };

public:
    void init();
    void shutdown();
    void clear();
    void flush();

    void draw_sprite(sprite* s);

    void push_scissors(const rect& r);
    void pop_scissors();

private:
    renderer* _cur_renderer;
    renderer* _renderers[MAX_RENDERER_TYPE];
    std::stack<rect> _scissors_stack;
};

NS_S2D_END

#endif
