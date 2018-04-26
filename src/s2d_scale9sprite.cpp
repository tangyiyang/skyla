#include "s2d_scale9sprite.h"

NS_S2D

void scale9sprite::init()
{
    node::init();
    _texture = nullptr;
    _draw_mode = SLICE;
    _inset_rect = {0, 0, 0, 0};
}

void scale9sprite::init(const char *tex_file)
{
    S2D_ASSERT(tex_file);
    node::init();

    set_texture(tex_file);
}

void scale9sprite::init(sprite_frame* frame)
{
    S2D_ASSERT(frame);
    node::init();

    this->set_texture(frame);
}

void scale9sprite::set_texture(const char* tex_file)
{

}

void scale9sprite::set_texture(sprite_frame* frame)
{

}

void scale9sprite::set_texture(texture* tex)
{
    
}

NS_S2D_END
