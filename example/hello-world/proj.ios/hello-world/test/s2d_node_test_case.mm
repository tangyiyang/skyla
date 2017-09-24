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

#include "s2d_node_test_case.h"
#include "s2d_test_common.h"

using namespace s2d;

void node_test::setup()
{
    _ctx = new context();
    _ctx->init(3, 1136, 640);
}

void node_test::tear_down()
{
    _ctx->shutdown();
    delete _ctx;
}

bool node_test::test_node_addchild_with_zorder()
{
    node* s = new node();
    s->init();
    s->set_pos(200, 200);

    _ctx->_root->add_child(s);
    _ctx->_root->update(1);

    affine_transform world = s->local_to_world();

    bool a = FLT_EQUAL(world.x, 200);
    bool b = FLT_EQUAL(world.y, 200);

//    bool a= (fabs((pos.x) - (200)) < FLT_EPSILON);
    return a && b;
}
