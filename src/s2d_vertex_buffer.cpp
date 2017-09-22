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

#include "s2d_vertex_buffer.h"
#include "s2d_util.h"
#include "s2d_gl_util.h"

NS_S2D

void sprite_vertex_buffer::init()
{
    _num_vertices = 0;
    _max_vertices = S2D_MAX_SPRITE_VERTEX_BUFFER_SIZE;
    _vertex_buffer = (pos_tex_color_vertex*)malloc(sizeof(pos_tex_color_vertex) * _max_vertices);

    glGenBuffers(1, &_vbo);
    S2D_ASSERT(_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_tex_color_vertex) * _max_vertices, nullptr, GL_DYNAMIC_DRAW);

    CHECK_GL_ERROR
}

bool sprite_vertex_buffer::append_quad(pos_tex_color_vertex* quad)
{
    if (_num_vertices + 6 > _max_vertices) {
        return true;
    }

    pos_tex_color_vertex* p = (pos_tex_color_vertex*)quad;

    int n = _num_vertices;
    for (int i = 0; i < 6; ++i) {
        _vertex_buffer[n+i] = *(p+i);
    }
    _num_vertices += 6;

    return false;
}

void sprite_vertex_buffer::submit()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * sizeof(pos_tex_color_vertex), _vertex_buffer, GL_DYNAMIC_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, _num_vertices * sizeof(pos_tex_color_vertex), _vertex_buffer);

    _num_vertices = 0;
    CHECK_GL_ERROR;
}

void sprite_vertex_buffer::shutdown()
{
    glDeleteBuffers(1, &_vbo);
    free(_vertex_buffer);
}

NS_S2D_END
