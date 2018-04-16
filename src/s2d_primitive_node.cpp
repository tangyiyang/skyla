#include "s2d_primitive_node.h"

NS_S2D

void primitive_node::draw(render_state* rs)
{
    if (!_visible) {
        return;
    }

    _world_transform = transform_to(this->get_root());

    rs->draw_primitive(this);
    node::draw(rs);
}

void primitive_node::add_line(const vec2& begin, const vec2& end, color_t color)
{
    _lines.emplace_back(begin, end, color);
}

void primitive_node::add_rect(const vec2& bottom_left, const vec2& top_right, color_t color)
{
    vec2 bottom_right = vec2::make(top_right.x, bottom_left.y);
    vec2 top_left = vec2::make(bottom_left.x, top_right.y);

    _lines.emplace_back(bottom_left, bottom_right, color);
    _lines.emplace_back(bottom_right, top_right, color);
    _lines.emplace_back(top_right, top_left, color);
    _lines.emplace_back(top_left, bottom_left, color);
}

NS_S2D_END
