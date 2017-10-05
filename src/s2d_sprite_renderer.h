/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#ifndef s2d_sprite_renderer_h
#define s2d_sprite_renderer_h

#include "s2d_common.h"
#include "s2d_program.h"
#include "s2d_texture.h"

NS_S2D

#define MAX_TEX_COORD ((1<<16)-1)
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
              pos_tex_color_vertex* quad,
              texture* tex);
    void flush();

private:
    void update_indexes();

private:
    program*              _program;
    texture*              _texture;
    pos_tex_color_vertex* _vertex_buffer;
    index_t*              _index_buffer;
    int32_t               _num_indexes;
    int32_t               _num_vertices;
    int32_t               _max_vertices;
    int32_t               _max_indexes;
    GLuint                _ibo;
    GLuint                _vbo;
    GLuint                _vao;
};

NS_S2D_END

#endif /* s2d_sprite_renderer_h */
