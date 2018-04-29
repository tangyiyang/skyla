#include "s2d_context.h"
#include "s2d_scale9sprite.h"

NS_S2D

#define DIRTY_BIT_TEXTURE   (1<<0)
#define DIRTY_BIT_RECT      (1<<1)
#define DIRTY_BIT_DRAW_MODE (1<<2)

void scale9sprite::init()
{
    node::init();
    texture_blend_protocol::init();

    _draw_mode = SLICE;
    _frame = nullptr;
    _origin_size = size::make(0, 0);
    _dirty = false;
}

bool scale9sprite::update(float dt)
{
    if (_dirty) {
        this->set_quad_with_frame(_texture, _frame);
        _dirty= false;
    }

    return node::update(dt);
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
    _dirty = true;
}

void scale9sprite::set_texture(const char* tex_file)
{
    texture* tex = context::C()->_texture_cache->load(tex_file);

    texture_blend_protocol::set_texture(tex);
    _origin_size = tex->_size;
    _dirty = true;
}

void scale9sprite::set_texture(sprite_frame* frame)
{

}

void scale9sprite::set_texture(texture* tex)
{
    
}

void scale9sprite::set_border(const border& border)
{
    _border = border;
    _dirty = true;
}

void scale9sprite::set_quad_with_frame(texture* tex, sprite_frame* frame)
{
    /*
     * Coord layout for the code below.
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
    if (frame) {

    } else {
        float tex_w = tex->_size.width;
        float tex_h = tex->_size.height;

        // position coords
        float x = _border.left;
        float y = _border.bottom;
        float w = _size.width  - _border.right - _border.left;
        float h = _size.height - _border.top   - _border.bottom;

        float ix = _border.left;
        float iy = _border.bottom;
        float iw = tex_w - _border.right - _border.left;
        float ih = tex_h - _border.top   - _border.bottom;

        // texture coords.
        float l = uniform_to_uint16(0.0f);
        float r = uniform_to_uint16(1.0f);
        float b = uniform_to_uint16(0.0f);
        float t = uniform_to_uint16(1.0f);
        
        float il = uniform_to_uint16((ix)      / tex_w);
        float ir = uniform_to_uint16((ix + iw) / tex_w);
        float ib = uniform_to_uint16((iy)      / tex_h);
        float it = uniform_to_uint16((iy + ih) / tex_h);

        // rect G:
        pos_tex_color_vertex::assign(_vertices + 0, 0, 0, l,  t,  COLOR_WHITE); // 0
        pos_tex_color_vertex::assign(_vertices + 1, 0, y, l,  it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 2, x, 0, il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 3, x, y, il, it, COLOR_WHITE); // 12

        // rect H:
        pos_tex_color_vertex::assign(_vertices + 4, x,   0, il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 5, x,   y, il, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 6, x+w, 0, ir, t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 7, x+w, y, ir, it, COLOR_WHITE); // 13

        // rect I:
        pos_tex_color_vertex::assign(_vertices + 8,  x+w,         0, ir, t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 9,  x+w,         y, ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 10, _size.width, 0, r,  t,  COLOR_WHITE); // 3
        pos_tex_color_vertex::assign(_vertices + 11, _size.width, y, r,  it, COLOR_WHITE); // 4

        // rect F:
        pos_tex_color_vertex::assign(_vertices + 12, x+w,         y,   ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 13, x+w,         y+h, ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 14, _size.width, y,   r,  it, COLOR_WHITE); // 4
        pos_tex_color_vertex::assign(_vertices + 15, _size.width, y+h, r,  ib, COLOR_WHITE); // 5

        // rect C:
        pos_tex_color_vertex::assign(_vertices + 16, x+w,         y+h,          ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 17, x+w,         _size.height, ir, b,  COLOR_WHITE); // 7
        pos_tex_color_vertex::assign(_vertices + 18, _size.width, y+h,          r,  ib, COLOR_WHITE); // 5
        pos_tex_color_vertex::assign(_vertices + 19, _size.width, _size.height, r,  b,  COLOR_WHITE); // 6

        // rect B:
        pos_tex_color_vertex::assign(_vertices + 20, x,   y+h,          il, ib, COLOR_WHITE); // 15
        pos_tex_color_vertex::assign(_vertices + 21, x,   _size.height, il, b,  COLOR_WHITE); // 8
        pos_tex_color_vertex::assign(_vertices + 22, x+w, y+h,          ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 23, x+w, _size.height, ir, b,  COLOR_WHITE); // 7

        // rect A:
        pos_tex_color_vertex::assign(_vertices + 24, 0, y+h,          l,  ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 25, 0, _size.height, l,  b,  COLOR_WHITE); // 9
        pos_tex_color_vertex::assign(_vertices + 26, x, y+h,          il, ib, COLOR_WHITE); // 15
        pos_tex_color_vertex::assign(_vertices + 27, x, _size.height, il, b,  COLOR_WHITE); // 8

        // rect D:
        pos_tex_color_vertex::assign(_vertices + 28, 0, y,   l,  it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 29, 0, y+h, l,  ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 30, x, y,   il, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 31, x, y+h, il, ib, COLOR_WHITE); // 15

        // rect E:
        pos_tex_color_vertex::assign(_vertices + 32, x,   y,   il, it, COLOR_WHITE); // 11
        pos_tex_color_vertex::assign(_vertices + 33, x,   y+h, il, ib, COLOR_WHITE); // 10
        pos_tex_color_vertex::assign(_vertices + 34, x+w, y,   ir, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 35, x+w, y+h, ir, ib, COLOR_WHITE); // 15
    }
}

NS_S2D_END
