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

void scale9sprite::set_texture(const char* tex_file)
{
    texture* tex = context::C()->_texture_cache->load(tex_file);

    texture_blend_protocol::set_texture(tex);

    this->set_quad_with_frame(tex, nullptr);
    _size = tex->_size;
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
        pos_tex_color_vertex::assign(_vertices + 1, ix, 0,  il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 2, ix, iy, il, it, COLOR_WHITE); // 12
        pos_tex_color_vertex::assign(_vertices + 3, 0,  iy, l,  it, COLOR_WHITE); // 11

        // rect H:
        pos_tex_color_vertex::assign(_vertices + 4, ix,      0,  il, t,  COLOR_WHITE); // 1
        pos_tex_color_vertex::assign(_vertices + 5, ix + iw, 0,  ir, t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 6, ix + iw, iy, ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 7, ix,      iy, il, it, COLOR_WHITE); // 12

        // rect I:
        pos_tex_color_vertex::assign(_vertices + 8, ix + iw,     0,  ir,t,  COLOR_WHITE); // 2
        pos_tex_color_vertex::assign(_vertices + 9, _size.width, 0,  r, t,  COLOR_WHITE); // 3
        pos_tex_color_vertex::assign(_vertices + 10, _size.width, iy, r, it, COLOR_WHITE); // 4
        pos_tex_color_vertex::assign(_vertices + 11, ix + iw,     iy, ir, it, COLOR_WHITE); // 13

        // rect F:
        pos_tex_color_vertex::assign(_vertices + 12, ix + iw,     iy, ir, it, COLOR_WHITE); // 13
        pos_tex_color_vertex::assign(_vertices + 13, _size.width, iy,  r, it,  COLOR_WHITE); // 4
        pos_tex_color_vertex::assign(_vertices + 14, _size.width, iy+ih, r, ib, COLOR_WHITE); // 5
        pos_tex_color_vertex::assign(_vertices + 15, ix + iw,     iy+ih, ir, ib, COLOR_WHITE); // 14

        // rect C:
        pos_tex_color_vertex::assign(_vertices + 16, ix + iw,     iy+ih, ir, ib, COLOR_WHITE); // 14
        pos_tex_color_vertex::assign(_vertices + 17, _size.width, iy+ih, r, ib,  COLOR_WHITE); // 5
        pos_tex_color_vertex::assign(_vertices + 18, _size.width, _size.height, r, b, COLOR_WHITE); // 6
        pos_tex_color_vertex::assign(_vertices + 19, ix + iw,     _size.height, ir, b, COLOR_WHITE); // 7
//
//        _vertices[1].pos.x = ix;
//        _vertices[1].pos.y = 0;
//        _vertices[1].uv.u  = il;
//        _vertices[1].uv.v  = t;
//
//        _vertices[2].pos.x = ix + iw;
//        _vertices[2].pos.y = 0;
//        _vertices[2].uv.u  = ir;
//        _vertices[2].uv.v  = t;
//
//        _vertices[3].pos.x = _size.width;
//        _vertices[3].pos.y = 0;
//        _vertices[3].uv.u  = r;
//        _vertices[3].uv.v  = t;
//
//        _vertices[4].pos.x = _size.width;
//        _vertices[4].pos.y = iy;
//        _vertices[4].uv.u  = r;
//        _vertices[4].uv.v  = it;
//
//        _vertices[5].pos.x = _size.width;
//        _vertices[5].pos.y = iy + ih;
//        _vertices[5].uv.u  = r;
//        _vertices[5].uv.v  = ib;
//
//        _vertices[6].pos.x = _size.width;
//        _vertices[6].pos.y = _size.height;
//        _vertices[6].uv.u  = r;
//        _vertices[6].uv.v  = b;
//
//        _vertices[7].pos.x = ix + iw;
//        _vertices[7].pos.y = _size.height;
//        _vertices[7].uv.u  = ir;
//        _vertices[7].uv.v  = b;
//
//        _vertices[8].pos.x = ix;
//        _vertices[8].pos.y = _size.height;
//        _vertices[8].uv.u  = il;
//        _vertices[8].uv.v  = b;
//
//        _vertices[9].pos.x = 0;
//        _vertices[9].pos.y = _size.height;
//        _vertices[9].uv.u  = l;
//        _vertices[9].uv.v  = b;
//
//        _vertices[10].pos.x = 0;
//        _vertices[10].pos.y = iy + ih;
//        _vertices[10].uv.u  = l;
//        _vertices[10].uv.v  = ib;
//
//        _vertices[11].pos.x = 0;
//        _vertices[11].pos.y = iy;
//        _vertices[11].uv.u  = l;
//        _vertices[11].uv.v  = it;
//
//        _vertices[12].pos.x = ix;
//        _vertices[12].pos.y = iy;
//        _vertices[12].uv.u  = il;
//        _vertices[12].uv.v  = it;
//
//        _vertices[13].pos.x = ix + iw;
//        _vertices[13].pos.y = iy;
//        _vertices[13].uv.u  = ir;
//        _vertices[13].uv.v  = it;
//
//        _vertices[14].pos.x = ix + iw;
//        _vertices[14].pos.y = iy + ih;
//        _vertices[14].uv.u  = ir;
//        _vertices[14].uv.v  = ib;
//
//        _vertices[15].pos.x = ix;
//        _vertices[15].pos.y = iy + ih;
//        _vertices[15].uv.u  = il;
//        _vertices[15].uv.v  = ib;
    }
}

NS_S2D_END
