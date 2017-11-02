#include "s2d_panel.h"

NS_S2D

void panel::init(const rect& r)
{
    _rect = r;
}

void panel::update(float dt)
{
    node::update(dt);
    
}

NS_S2D_END
