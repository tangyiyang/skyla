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

#include "s2d_context.h"
#include "s2d_gl_util.h"
#include "s2d_util.h"

NS_S2D

context* context::_global_context = nullptr;

void context::init(int opengles_version, float width, float height)
{
    _global_context = this;
    s2d::gl_util::check_gl_extension(opengles_version);

    _sprite_renderer = new sprite_renderer();
    _file_system = new file_system();
    _camera = new camera();
    _root = new node();

    _sprite_renderer->init();
    _file_system->init();
    _camera->init_orthographic(width, height);
    _root->init();

    sprite* logo = new sprite();
    logo->init();
    _root->add_child(logo);
    
    _world_view_affine_transform = affine_transform::mk_translate(-width/2, -height/2);

    LOGD("context initialized.");
    if (_app) {
        _app->on_init(this);
    }
}

void context::loop(float dt)
{
    _root->update(dt);
    _sprite_renderer->flush();
}

void context::pause()
{
    LOGD("context paused.");
    if (_app) {
        _app->on_pause();
    }
}

void context::resume()
{
    LOGD("context resumed.");
    if (_app) {
        _app->on_resume();
    }
}

void context::shutdown()
{
    LOGD("context will shutdown, good bye :).");
    _sprite_renderer->shutdown();
    _root->remove_all_children();

    delete _sprite_renderer;
    delete _file_system;
    delete _root;
    delete _camera;

    if (_app) {
        _app->on_destroy();
    }
}


NS_S2D_END
