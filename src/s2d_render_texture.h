#ifndef s2d_render_texture_h
#define s2d_render_texture_h

#include "s2d_node.h"

NS_S2D

class render_texture : public node
{
public:
    void init();
    void update(float dt);
};

NS_S2D_END

#endif /* s2d_render_texture_h */
