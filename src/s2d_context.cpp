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

void context::init(int opengles_version, float window_width, float window_height)
{
    _global_context = this;
    s2d::gl_util::check_gl_extension(opengles_version);

    init_resolution_settings(window_width, window_height);
    init_fundamental_components(_logic_size);
    LOGD("context initialized.");

    if (_app) {
        _app->on_init(this);
    }
}

void context::init_resolution_settings(float window_width, float window_height)
{
    _logic_size = {1136, 640};
    _window_size = {window_width, window_height};
    _resolution_compat_type = FIXED_WIDTH;

    switch (_resolution_compat_type) {
        case FIXED_WIDTH:
            _scale_factor.y = _scale_factor.x = _window_size.width / _logic_size.width;
            _logic_size.height = ceilf(_window_size.height / _scale_factor.y);
            _visible_rect.origin = {0, 0};
            _visible_rect.size =_logic_size;
            break;

        case FIXED_HEIGHT:
            _scale_factor.x = _scale_factor.y = _window_size.height / _logic_size.height;
            _logic_size.width = ceilf(_window_size.width / _scale_factor.x);
            _visible_rect.origin = {0, 0};
            _visible_rect.size =_logic_size;
        default:
            break;
    }

    _viewport_rect.size.width = _logic_size.width * _scale_factor.x;
    _viewport_rect.size.height = _logic_size.height * _scale_factor.y;
    _viewport_rect.origin.x = (_window_size.width - _viewport_rect.size.width)/2;
    _viewport_rect.origin.y = (_window_size.height - _viewport_rect.size.height)/2;

    LOGD("** resolution compat mode = %d", _resolution_compat_type);
    LOGD("** window size = {%.2f, %.2f}", _window_size.width, _window_size.height);
    LOGD("** viewport size = {%.2f, %.2f}", _viewport_rect.size.width, _viewport_rect.size.height);
    LOGD("** _logic_size size = {%.2f, %.2f}", _logic_size.width, _logic_size.height);
    LOGD("** visible origin = {%.2f, %.2f}", _visible_rect.origin.x, _visible_rect.origin.y);
    LOGD("** visible size = {%.2f, %.2f}", _visible_rect.size.width, _visible_rect.size.height);
}

void context::init_fundamental_components(const size& logic_size)
{
    _sprite_frame_cache = new sprite_frame_cache();
    _sprite_renderer = new sprite_renderer();
    _file_system = new file_system();
    _camera = new camera();
    _root = new node();

    _sprite_renderer->init();
    _file_system->init();
    _camera->init_orthographic(logic_size.width, logic_size.height);
    _root->init();

    _world_view_affine_transform = affine_transform::mk_translate(-logic_size.width/2,
                                                                  -logic_size.height/2);
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
    _sprite_frame_cache->shutdown();
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
