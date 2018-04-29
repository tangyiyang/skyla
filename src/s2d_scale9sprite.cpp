#include "s2d_context.h"
#include "s2d_scale9sprite.h"

NS_S2D

void scale9sprite::init()
{
    node::init();
    texture_blend_protocol::init();

    _draw_mode = SLICE;
    _inset_rect = {0, 0, 0, 0};
}

void scale9sprite::draw(render_state* rs)
{
    if (!_visible) {
        return;
    }
    _world_transform = transform_to(this->get_root());

    if (_texture) {
        rs->draw_scale9sprite(this);
    }

    node::draw(rs);
}

void scale9sprite::set_size(const size& size)
{
    _size = size;
    this->set_quad_with_frame(_texture, nullptr);
}

void scale9sprite::set_texture(const char* tex_file)
{
    texture* tex = context::C()->_texture_cache->load(tex_file);

    texture_blend_protocol::set_texture(tex);

    _size = tex->_size;
    _inset_rect = {_size.width/3, _size.height/3, _size.width/3, _size.height/3};
    this->set_quad_with_frame(tex, nullptr);
}

void scale9sprite::set_texture(sprite_frame* frame)
{

}

void scale9sprite::set_texture(texture* tex)
{
    
}

void scale9sprite::set_quad_with_frame(texture* tex, sprite_frame* frame)
{
    if (frame) {

    } else {
        /*
         * Slice Mode, full texture.
         *
         *    9--8-----------7--6
         *    |A |     B     | C|
         *    10-15---------14--5
         *    |  |           |  |
         *    |  |           |  |
         *    |D |     E     | F|
         *    |  |           |  |
         *    |  |           |  |
         *    11-12---------13--4
         *    |G |     H     | I|
         *    0--1-----------2--3
         */

        float tex_w = tex->_size.width;
        float tex_h = tex->_size.height;

        float ix = _inset_rect.origin.x;
        float iy = _inset_rect.origin.y;
        float iw = _inset_rect.size.width;
        float ih = _inset_rect.size.height;

        float l = uniform_to_uint16(0.0f);
        float r = uniform_to_uint16(1.0f);
        float b = uniform_to_uint16(0.0f);
        float t = uniform_to_uint16(1.0f);

        float il = uniform_to_uint16((ix)      / tex_w);
        float ir = uniform_to_uint16((ix + iw) / tex_w);
        float ib = uniform_to_uint16((iy)      / tex_h);
        float it = uniform_to_uint16((iy + ih) / tex_h);

        // rect G:
        pos_tex_color_vertex::assign(_vertices + 0, 0,  0,  l,  t,  COLOR_WHITE); // 0
        pos_tex_color_vertex::assign(_vertices + 1, 0,  iy, l,  it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 2, ix, 0,  il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 3, ix, iy, il, it, COLOR_WHITE); // 12


        // rect H:
        pos_tex_color_vertex::assign(_vertices + 4, ix,    0,  il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 5, ix,    iy, il, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 6, ix+iw, 0,  ir, t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 7, ix+iw, iy, ir, it, COLOR_WHITE); // 13


        // rect I:
        pos_tex_color_vertex::assign(_vertices + 8,  ix+iw,       0,  ir, t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 9,  ix+iw,       iy, ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 10, _size.width, 0,  r,  t,  COLOR_WHITE); // 3
        pos_tex_color_vertex::assign(_vertices + 11, _size.width, iy, r,  it, COLOR_WHITE); // 4


        // rect F:
        pos_tex_color_vertex::assign(_vertices + 12, ix+iw,       iy,    ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 13, ix+iw,       iy+ih, ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 14, _size.width, iy,    r,  it, COLOR_WHITE); // 4
        pos_tex_color_vertex::assign(_vertices + 15, _size.width, iy+ih, r,  ib, COLOR_WHITE); // 5


        // rect C:
        pos_tex_color_vertex::assign(_vertices + 16, ix+iw,       iy+ih,        ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 17, ix+iw,       _size.height, ir, b,  COLOR_WHITE); // 7
        pos_tex_color_vertex::assign(_vertices + 18, _size.width, iy+ih,        r,  ib, COLOR_WHITE); // 5
        pos_tex_color_vertex::assign(_vertices + 19, _size.width, _size.height, r,  b,  COLOR_WHITE); // 6


        // rect B:
        pos_tex_color_vertex::assign(_vertices + 20, ix,    iy+ih,        il, ib, COLOR_WHITE); // 15
        pos_tex_color_vertex::assign(_vertices + 21, ix,    _size.height,  il, b,  COLOR_WHITE); // 8
        pos_tex_color_vertex::assign(_vertices + 22, ix+iw, iy+ih,        ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 23, ix+iw, _size.height, ir, b,  COLOR_WHITE); // 7


        // rect A:
        pos_tex_color_vertex::assign(_vertices + 24, 0,  iy+ih,        l,  ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 25, 0,  _size.height, l,  b,  COLOR_WHITE); // 9
        pos_tex_color_vertex::assign(_vertices + 26, ix, iy+ih,        il, ib, COLOR_WHITE); // 15
        pos_tex_color_vertex::assign(_vertices + 27, ix, _size.height, il, b,  COLOR_WHITE); // 8


        // rect D:
        pos_tex_color_vertex::assign(_vertices + 28, 0,  iy,    l,  it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 29, 0,  iy+ih, l,  ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 30, ix, iy,    il, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 31, ix, iy+ih, il, ib, COLOR_WHITE); // 15


        // rect E:
        pos_tex_color_vertex::assign(_vertices + 32, ix,    iy,    il, it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 33, ix,    iy+ih, il, ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 34, ix+iw, iy,    ir, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 35, ix+iw, iy+ih, ir, ib, COLOR_WHITE); // 15
    }
}

NS_S2D_END
