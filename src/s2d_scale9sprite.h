/*
 * Scale9sprite,
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_scale9sprite_h
#define s2d_scale9sprite_h

#include "s2d_sprite.h"

NS_S2D

class scale9sprite : public node,
                     public texture_blend_protocol
{
public:
    enum DRAW_MODE {
        SLICE = 0,
        TILED,
    };

public:
    scale9sprite();
    virtual ~scale9sprite();

public:
    void init();
    void init(const char* tex_file);
    void init(sprite_frame* frame);
    void init(texture* tex);

public:
    void set_texture(const char* tex_file);
    void set_texture(sprite_frame* frame);
    void set_texture(texture* tex);

    inline void set_draw_mode(DRAW_MODE mode)
    {
        _draw_mode = mode;
    }

    inline void set_inset_rect(const rect& r)
    {
        _inset_rect = r;
    }

public:
    pos_tex_color_vertex _quad[9];
    DRAW_MODE            _draw_mode;
    rect                 _inset_rect;
};

NS_S2D_END

#endif
