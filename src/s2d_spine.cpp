#include "s2d_spine.h"

NS_S2D

void spine_anim::init()
{

}

bool spine_anim::update(float dt)
{
    
    return node::update(dt);
}

void spine_anim::draw(render_state* rs)
{

}

void spine_anim::hit_test(touch_handler* handler, touch_event* event)
{

}

void spine_anim::on_touch(touch_event* event)
{

}

rect spine_anim::bounds_in(node* space)
{
    return node::bounds_in(this);
}

NS_S2D_END
