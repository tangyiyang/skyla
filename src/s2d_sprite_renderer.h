/*
 * Sprite renderer, draw quads or triangles with this.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_sprite_renderer_h
#define s2d_sprite_renderer_h

#include "s2d_common.h"
#include "s2d_program.h"
#include "s2d_texture.h"

NS_S2D

typedef uint16_t index_t;
struct uv_t {
    uint16_t u;
    uint16_t v;
};

struct pos_tex_color_vertex {
    vec2 pos;
    uv_t uv;
    uint32_t color;
};

union sprite_quad {
    struct {
        pos_tex_color_vertex tl;
        pos_tex_color_vertex tr;
        pos_tex_color_vertex br;
        pos_tex_color_vertex bl;
    };
    pos_tex_color_vertex v[4];
};

class sprite_renderer {
public:
    sprite_renderer();

public:
    void init();
    void shutdown();
    void draw(const affine_transform& world_transform,
                   texture* tex,
                   pos_tex_color_vertex* vertices,
                   int n);

    void flush();

private:
    void update_indexes();

private:
    program*              _program;
    texture*              _texture;
    index_t*              _index_buffer;
    pos_tex_color_vertex* _vertex_buffer;
    int32_t               _num_indexes;
    int32_t               _num_vertices;
    int32_t               _max_vertices;
    int32_t               _max_indexes;
    GLuint                _ibo;
    GLuint                _vbo;
    GLuint                _vao;
};

NS_S2D_END

#endif
