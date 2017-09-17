/****************************************************************************
Copyright (c) Yiyang Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef s2d_vertex_buffer_h
#define s2d_vertex_buffer_h

#include "s2d_common.h"

NS_S2D

struct uv_t {
    uint16_t u;
    uint16_t v;
};

struct vertex_attr {
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    GLvoid* offset;
};

struct pos_tex_color_vertex {
    vec2 pos;
    uv_t uv;
    uint32_t color;

    static vertex_attr* attr()
    {
        static vertex_attr _attr[3] = {
            {.size = 2,
             .type = GL_FLOAT,
             .normalized = false,
             .stride = sizeof(vertex_attr),
             .offset = (void*)offsetof(pos_tex_color_vertex, pos)},

            {.size = 2,
             .type = GL_UNSIGNED_SHORT,
             .normalized = false,
             .stride = sizeof(vertex_attr),
             .offset = (void*)offsetof(pos_tex_color_vertex, uv)},

            {.size = 4,
             .type = GL_BYTE,
             .normalized = false,
             .stride = sizeof(vertex_attr),
             .offset = (void*)offsetof(pos_tex_color_vertex, color)}
        };
        return _attr;
    }
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

class sprite_vertex_buffer {
public:
    void init();
    bool append_quad(pos_tex_color_vertex quad[4]);

private:
    std::vector<pos_tex_color_vertex> _vertex_buffer;
    int32_t                           _num_vertices;
    int32_t                           _max_vertices;
};

NS_S2D_END

#endif /* s2d_vertex_buffer_h */
