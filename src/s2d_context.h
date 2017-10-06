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

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_sprite.h"
#include "s2d_camera.h"
#include "s2d_file_system.h"
#include "s2d_sprite_renderer.h"

NS_S2D

class context;
class app_protocol {
public:
    virtual void on_init(context* ctx) = 0;
    virtual void on_pause() = 0;
    virtual void on_resume() = 0;
    virtual void on_destroy() = 0;
    virtual void on_resize(context* ctx) = 0;
};

class context {
public:
    enum RESOLUTION_COMPAT_TYPE {
        FIXED_WIDTH = 0,
        FIXED_HEIGHT = 1,
    };
public:
    context(app_protocol* protocol = nullptr) : _app(protocol) {}

public:
    void init(int opengles_version, float window_width, float window_height);
    void clear();
    void loop(float dt);
    void pause();
    void resume();
    void shutdown();

    void on_touch_begin(uint64_t id, float x, float y);
    void on_touch_moved(uint64_t id, float x, float y);
    void on_touch_ended(uint64_t id, float x, float y);
    void on_touch_cancl(uint64_t id, float x, float y);

    void set_logic_size(float width, float height);
    void set_content_scale_factor(float factor); // for engine internal usage only!

    inline node* get_root()
    {
        return _root;
    }

    inline const rect& get_visible_rect()
    {
        return _visible_rect;
    }

private:
    void update_resolution_settings(float window_width, float window_height);
    void init_fundamental_components(const size& logic_size);

public:
    app_protocol*    _app;
    file_system*     _file_system;
    sprite_frame_cache* _sprite_frame_cache;
    sprite_renderer* _sprite_renderer;
    node*            _root;
    camera*          _camera;

    affine_transform _world_view_affine_transform;

    size             _logic_size;
    size             _window_size;
    rect             _viewport_rect;
    rect             _visible_rect;
    vec2             _scale_factor;
    RESOLUTION_COMPAT_TYPE _resolution_compat_type;
    float            _content_scale_factor; // on retina screen, this may be 2 or 3.
    
public:
    static inline context* C()
    {
        return _global_context;
    }
private:
    /* The ONLY static context in seal2d. */
    static context* _global_context;
};

NS_S2D_END

#endif /* s2d_context_h */
