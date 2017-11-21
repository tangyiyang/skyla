/*
 * Spine is the right choice for skeleton animation. :)
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_spine_h
#define s2d_spine_h

#ifdef S2D_ENABLE_SPINE

#include "spine/spine.h"
#include "s2d_common.h"
#include "s2d_node.h"

NS_S2D

class spine_anim : public node {
public:
    void init() override;
    bool update(float dt) override;
    void draw(render_state* rs) override;
    void hit_test(touch_handler* handler, touch_event* event) override;
    void on_touch(touch_event* event) override;
    rect bounds_in(node* space) override;

private:

};

NS_S2D_END

#endif /* S2D_ENABLE_SPINE */

#endif
