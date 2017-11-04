#include "s2d_panel.h"

NS_S2D

void panel::init(const rect& r)
{
    node::init();
    _rect = r;
}

void panel::update(float dt)
{
    node::update(dt);
}

void panel::draw(render_state* rs)
{
    affine_transform transform = transform_to(this->get_root());

    rect _world_rect = affine_transform::apply_transform(transform, _rect);
    rs->push_scissors(_world_rect);
    rs->flush();

    node::draw(rs);

    rs->flush();
    rs->pop_scissors();
}

NS_S2D_END
