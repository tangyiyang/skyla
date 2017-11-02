#ifndef render_state_h
#define render_state_h

#include "s2d_renderer.h"
#include "s2d_sprite.h"

NS_S2D

class render_state {
public:
    enum RENDERER_TYPE {
        RENDERER_TYPE_QUAD = 0,
        MAX_RENDERER_TYPE,
    };

public:
    void init();
    void shutdown();
    void flush();

    void draw_sprite(sprite* s);

private:
    renderer* _cur_renderer;
    renderer* _renderers[MAX_RENDERER_TYPE];
    std::stack<rect> _scissors_stack;
};

NS_S2D_END
#endif /* render_state_hpp */
