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

#include "s2d_sprite.h"
#include "s2d_context.h"
#include "s2d_util.h"

NS_S2D

void sprite::init()
{
    _local_transform = affine_transform::mk_identity();
    _pos = {0, 0};
    
    _scale = {1.0, 1.0};
    _rotation = 0;
    _anchor = {0, 0};
    _size = {200*_scale.x, 400};

    _quad[0].pos.x = 0;
    _quad[0].pos.y = 0;
    _quad[0].uv.u = 0;
    _quad[0].uv.v = 0;
    _quad[0].color = 0xffffff;

    _quad[1].pos.x = 0;
    _quad[1].pos.y = 158;
    _quad[1].uv.u = 0;
    _quad[1].uv.v = (1 <<16)-1;
    _quad[1].color = 0xffffff;

    _quad[2].pos.x = 256;
    _quad[2].pos.y = 0;
    _quad[2].uv.u = (1 <<16)-1;
    _quad[2].uv.v = 0;
    _quad[2].color = 0xffffff;

    _quad[3].pos.x = 256;
    _quad[3].pos.y = 158;

    _quad[3].uv.u = (1<<16)-1;
    _quad[3].uv.v = (1<<16)-1;

    _quad[3].color = 0xffffff;

    _program = program::load_default_program(program::EMBEDED_PROGRAM_SPRITE_DEFAULT);

    _vbo = 0;
    _vao = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    GLuint loc_pos =  _program->enable_attribute("pos");
    GLuint loc_texcoord =  _program->enable_attribute("tex_coord");
    GLuint loc_color = _program->enable_attribute("color");

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(loc_pos);
    glEnableVertexAttribArray(loc_texcoord);
    glEnableVertexAttribArray(loc_color);

    glVertexAttribPointer(loc_pos, 2, GL_FLOAT, GL_TRUE, sizeof(pos_tex_color_vertex), (void*)offsetof(pos_tex_color_vertex, pos));
    glVertexAttribPointer(loc_texcoord, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(pos_tex_color_vertex), (void*)offsetof(pos_tex_color_vertex, uv));
    glVertexAttribPointer(loc_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(pos_tex_color_vertex), (void*)offsetof(pos_tex_color_vertex, color));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _texture = new texture();
    _texture->init("res/seal2d-opacity-half.png");
}

void sprite::update_srt()
{
    _local_transform = affine_transform::mk_identity();

    float anchor_x = _size.width * _anchor.x;
    float anchor_y = _size.height * _anchor.y;
// TODO: optimize this code
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotation);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);
    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);

    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
    affine_transform::inplace_concat(_local_transform, anchor_to);
}

void sprite::update()
{
    this->update_srt();

    context* ctx = context::_global_context;

    const affine_transform& mv = ctx->_world_view_affine_transform;
    affine_transform t = affine_transform::concat(_local_transform, mv);

    
    _vertex[0].pos = affine_transform::apply_transform(t, _quad[0].pos.x, _quad[0].pos.y);
    _vertex[1].pos = affine_transform::apply_transform(t, _quad[1].pos.x, _quad[1].pos.y);
    _vertex[2].pos = affine_transform::apply_transform(t, _quad[2].pos.x, _quad[2].pos.y);
    _vertex[3].pos = affine_transform::apply_transform(t, _quad[2].pos.x, _quad[2].pos.y);
    _vertex[4].pos = affine_transform::apply_transform(t, _quad[3].pos.x, _quad[3].pos.y);
    _vertex[5].pos = affine_transform::apply_transform(t, _quad[1].pos.x, _quad[1].pos.y);
    
    _vertex[0].color = _quad[0].color;
    _vertex[1].color = _quad[1].color;
    _vertex[2].color = _quad[2].color;
    _vertex[3].color = _quad[2].color;
    _vertex[4].color = _quad[3].color;
    _vertex[5].color = _quad[1].color;
    
    _vertex[0].uv = _quad[0].uv;
    _vertex[1].uv = _quad[1].uv;
    _vertex[2].uv = _quad[2].uv;
    _vertex[3].uv = _quad[2].uv;
    _vertex[4].uv = _quad[3].uv;
    _vertex[5].uv = _quad[1].uv;

    this->draw();
}

void sprite::draw()
{
    context::_global_context->_sprite_renderer->draw(_vertex);
}

NS_S2D_END
