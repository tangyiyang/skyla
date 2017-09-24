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

#include "s2d_sprite_renderer.h"
#include "s2d_context.h"

NS_S2D

sprite_renderer::sprite_renderer() :
_program(nullptr),
_texture(nullptr),
_vertex_buffer(nullptr),
_index_buffer(nullptr),
_vbo(0),
_ibo(0),
_vao(0),
_num_indexes(0),
_max_indexes(0),
_num_vertices(0),
_max_vertices(0)
{

}

void sprite_renderer::init()
{

    CHECK_GL_ERROR
    _num_vertices = 0;
    _max_vertices = S2D_MAX_SPRITE_VERTEX_BUFFER_SIZE;
    _num_indexes = 0;
    _max_indexes = _max_vertices/4*6;
    _vertex_buffer = (pos_tex_color_vertex*)malloc(sizeof(pos_tex_color_vertex) * _max_vertices);
    _index_buffer = (index_t*)malloc(sizeof(index_t) * _max_indexes);

    _program = program::load_default_program(program::EMBEDED_PROGRAM_SPRITE_DEFAULT);
    CHECK_GL_ERROR
    GLuint loc_pos = _program->enable_attribute("pos");
    GLuint loc_texcoord = _program->enable_attribute("tex_coord");
    GLuint loc_color = _program->enable_attribute("color");
    glGenBuffers(1, &_vbo); S2D_ASSERT(_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_tex_color_vertex) * _max_vertices, nullptr, GL_DYNAMIC_DRAW);

    CHECK_GL_ERROR

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t) * _max_indexes, nullptr, GL_DYNAMIC_DRAW);
    
    if (gl_util::support_vao()) {
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glEnableVertexAttribArray(loc_pos);
        glEnableVertexAttribArray(loc_texcoord);
        glEnableVertexAttribArray(loc_color);
        setup_vertex_attr();

        glBindVertexArray(0);
        CHECK_GL_ERROR;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sprite_renderer::shutdown()
{
    _program->shutdown();
    
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
    glDeleteVertexArrays(1, &_vao);
    free(_vertex_buffer);
    free(_index_buffer);
}

void sprite_renderer::draw(const affine_transform& world_transform,
                           pos_tex_color_vertex* quad,
                           texture* tex)
{
    if (_texture == nullptr) {
        _texture = tex;
    } else {
        if (_texture != tex) {
            this->flush();
            _texture = tex;
        }
    }

    if (_num_vertices + 4 > _max_vertices) {
        this->flush();
    }
    
    context* ctx = context::C();
    const affine_transform& mv = ctx->_world_view_affine_transform;
    affine_transform t = affine_transform::concat(world_transform, mv);

    pos_tex_color_vertex* p = _vertex_buffer + _num_vertices;
    for (int i = 0; i < 4; ++i) {
        (p+i)->pos = affine_transform::apply_transform(t, quad[i].pos.x, quad[i].pos.y);
        (p+i)->color = quad[i].color;
        (p+i)->uv = quad[i].uv;
    }
    
    _num_vertices += 4;
}

void sprite_renderer::flush()
{
    if (!_program || !_texture) {
        return;
    }
    
    this->update_indexes();
    _program->use();
    _texture->bind();
    CHECK_GL_ERROR;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _num_vertices * sizeof(pos_tex_color_vertex), _vertex_buffer);
    
    _program->set_uniform("u_projection",
                          program::UNIFORM_TYPE_MATRIX_3_FV,
                          context::C()->_camera->_matrix.m);

    if(gl_util::support_vao()) {
        glBindVertexArray(_vao);
    } else {
        setup_vertex_attr();
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _num_indexes * sizeof(index_t), _index_buffer);
    glDrawElements(GL_TRIANGLES, _num_indexes, GL_UNSIGNED_SHORT, 0);
    CHECK_GL_ERROR;

    _program->unuse();
    _texture->unbind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (gl_util::support_vao()) {
        glBindVertexArray(0);
    }
    _num_indexes = 0;
    _num_vertices = 0;
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

void sprite_renderer::update_indexes()
{
    for (int i = 0, j = 0; i < _num_vertices; i += 4, j+=6) {
        _index_buffer[j+0] = j+0;
        _index_buffer[j+1] = j+1;
        _index_buffer[j+2] = j+2;
        _index_buffer[j+3] = j+2;
        _index_buffer[j+4] = j+3;
        _index_buffer[j+5] = j+1;
    }
    _num_indexes = _num_vertices/4*6;
}

NS_S2D_END
