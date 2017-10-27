/*
 * Sprite for most users, you may create a sprite by a whole texture or atlas.
 * Atlas is generated with TexturePacker: https://www.codeandweb.com/texturepacker
 * With the V-Play format.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_sprite_h
#define s2d_sprite_h

#include "s2d_node.h"
#include "s2d_texture.h"
#include "s2d_program.h"
#include "s2d_sprite_renderer.h"

NS_S2D

class sprite_frame : public ref_counter {
public:
    rect _frame;
    rect _sprite_source_rect;
    size _source_size;
    bool _rotated;
    bool _trimmed;
    std::string _name;
    texture* _texture;
};

class sprite_frame_cache {
public:
    void load(const char* json_atlas, const char* texture_file);
    sprite_frame* get(const char* name);

    void shutdown();
private:
    std::map<std::string, sprite_frame*> _cache;
};

class sprite: public node {
    /*
     * The Quad Sprite.
     */
public:
    void init(const char* tex_file);
    void init(sprite_frame* frame);
    void init(const rect& r, texture* tex);
    void init(texture* tex);
    void update(float dt)       override;
    rect bounds_in(node* space) override;

public:
    void set_color(uint32_t color);

protected:
    void draw();
    void update_srt();

private:
    void setTextureCoord(const rect& r, texture* tex);
    void setTextureCoord(sprite_frame* frame, texture* tex);

protected:
    pos_tex_color_vertex _quad[4];
    texture*             _texture;
};

NS_S2D_END

#endif
