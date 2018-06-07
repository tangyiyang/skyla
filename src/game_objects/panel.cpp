#include "panel.h"

NS_SKYLA

void panel::init(const rect& r)
{
    node::init();
    _rect = r;
}

void panel::init(float x, float y, float w, float h)
{
    node::init();
    _rect.origin.x = x;
    _rect.origin.y = y;
    _rect.size.width = w;
    _rect.size.height = h;
}

bool panel::update(float dt)
{
    return node::update(dt);
}

void panel::draw(render_state* rs)
{
    affine_transform transform = transform_to(this->get_root());

    rs->flush();

    rect _world_rect = affine_transform::apply_transform(transform, _rect);
    rs->push_scissors(_world_rect);

    node::draw(rs);

    rs->flush();
    rs->pop_scissors();
}

NS_SKYLA_END
