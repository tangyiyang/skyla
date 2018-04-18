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
    LOGD("origin = %x, color = %x",color,  make_gl_color(color));
    _lines.emplace_back(begin, end, make_gl_color(color));
}

void primitive_node::add_rect(const vec2& bottom_left, const size& size, color_t color)
{
    vec2 top_right = vec2::make(bottom_left.x + size.width, bottom_left.y + size.height);
    vec2 bottom_right = vec2::make(top_right.x, bottom_left.y);
    vec2 top_left = vec2::make(bottom_left.x, top_right.y);

    color_t gl_color = make_gl_color(color);
    _lines.emplace_back(bottom_left, bottom_right, gl_color);
    _lines.emplace_back(bottom_right, top_right, gl_color);
    _lines.emplace_back(top_right, top_left, gl_color);
    _lines.emplace_back(top_left, bottom_left, gl_color);
}

NS_S2D_END
