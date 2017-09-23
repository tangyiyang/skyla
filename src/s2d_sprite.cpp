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
    node::init();

    _quad[0].pos.x = 0;
    _quad[0].pos.y = 0;
    _quad[0].uv.u = 0;
    _quad[0].uv.v = MAX_TEX_COORD;
    _quad[0].color = 0xffffff;

    _quad[1].pos.x = 0;
    _quad[1].pos.y = 158;
    _quad[1].uv.u = 0;
    _quad[1].uv.v = MAX_TEX_COORD - MAX_TEX_COORD;
    _quad[1].color = 0xffffff;

    _quad[2].pos.x = 256;
    _quad[2].pos.y = 0;
    _quad[2].uv.u = (1 <<16)-1;
    _quad[2].uv.v = MAX_TEX_COORD;
    _quad[2].color = 0xffffff;

    _quad[3].pos.x = 256;
    _quad[3].pos.y = 158;

    _quad[3].uv.u = (1<<16)-1;
    _quad[3].uv.v = MAX_TEX_COORD - MAX_TEX_COORD;

    _quad[3].color = 0xffffff;

    _texture = new texture();
    _texture->init("res/seal2d-transparent.png");
}

void sprite::update(float dt)
{
    node::update(dt);
    this->draw();
}

void sprite::draw()
{
    context::_global_context->_sprite_renderer->draw(
                                                     _local_transform,
                                                     _quad,
                                                     _texture);
}

NS_S2D_END
