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

        float ox = _inset_rect.origin.x;
        float oy = _inset_rect.origin.y;
        float ow = _inset_rect.size.width;
        float oh = _inset_rect.size.height;

    }
}

NS_S2D_END
