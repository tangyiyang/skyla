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

#include "s2d_sprite_renderer.h"
#include "s2d_context.h"

NS_S2D

sprite_renderer::sprite_renderer() :
_vertex_buffer(nullptr),
_program(nullptr),
_vao(0)
{

}

void sprite_renderer::init()
{
    _program = program::load_default_program(program::EMBEDED_PROGRAM_SPRITE_DEFAULT);
    _program->enable_attribute("pos");
    _program->enable_attribute("tex_coord");
    _program->enable_attribute("color");
    
    _num_vertices = 0;
    _max_vertices = S2D_MAX_SPRITE_VERTEX_BUFFER_SIZE;
    _vertex_buffer = (pos_tex_color_vertex*)malloc(sizeof(pos_tex_color_vertex) * _max_vertices);
    
    glGenBuffers(1, &_vbo); S2D_ASSERT(_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_tex_color_vertex) * _max_vertices, nullptr, GL_DYNAMIC_DRAW);
    
    CHECK_GL_ERROR
    
    if (gl_util::support_vao()) {
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        setup_vertex_attr();

        CHECK_GL_ERROR;
    }

}

void sprite_renderer::shutdown()
{
    _program->shutdown();
    
    glDeleteBuffers(1, &_vbo);
    free(_vertex_buffer);
}

void sprite_renderer::draw(pos_tex_color_vertex* quad)
{
    if (_num_vertices + 6 > _max_vertices) {
        this->flush();
    }
    
    pos_tex_color_vertex* p = (pos_tex_color_vertex*)quad;
    
    int n = _num_vertices;
    for (int i = 0; i < 6; ++i) {
        _vertex_buffer[n+i] = *(p+i);
    }
    _num_vertices += 6;
}

void sprite_renderer::flush()
{
    _program->use();

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _num_vertices * sizeof(pos_tex_color_vertex), _vertex_buffer);

    _program->set_uniform("u_projection",
                          program::UNIFORM_TYPE_MATRIX_3_FV,
                          context::_global_context->_camera->_matrix.m);

    if(gl_util::support_vao()) {
        glBindVertexArray(_vao);
    } else {
        setup_vertex_attr();
    }
    CHECK_GL_ERROR;
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void sprite_renderer::setup_vertex_attr()
{
    glVertexAttribPointer(_program->get_vertex_attr_location("pos"),
                          2,
                          GL_FLOAT,
                          GL_TRUE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, pos));

    glVertexAttribPointer(_program->get_vertex_attr_location("tex_coord"),
                          2,
                          GL_UNSIGNED_SHORT,
                          GL_TRUE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, uv));

    glVertexAttribPointer(_program->get_vertex_attr_location("color"),
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, color));
}

NS_S2D_END
