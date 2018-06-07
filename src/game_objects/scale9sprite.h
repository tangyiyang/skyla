/*
 * Scale9sprite,
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_scale9sprite_h
#define skyla_scale9sprite_h

#include "sprite.h"

NS_SKYLA

class scale9sprite : public node,
                     public texture_blend_protocol
{
public:
    enum DRAW_MODE {
        SLICE = 0,
        TILED,
    };

public:
    scale9sprite() {}
    virtual ~scale9sprite() {}

public:
    void init() override;
    bool update(float dt) override;
    void draw(render_state* rs) override;

public:
    void set_texture(texture* tex);
    void set_texture_with_file(const char* tex_file);
    void set_texture_with_frame(sprite_frame* frame);
    void set_size(float width, float height);

    void set_draw_mode(DRAW_MODE mode);
    void set_border(float left, float right, float bottom, float top);

private:
    void update_vertices();
    void update_vertices_in_slice_mode();
    void update_vertices_in_tile_mode();

public:
    pos_tex_color_vertex _vertices[36];
    DRAW_MODE            _draw_mode;
    border               _border;
    sprite_frame*        _frame;
    bool                 _dirty;
};

NS_SKYLA_END

#endif
