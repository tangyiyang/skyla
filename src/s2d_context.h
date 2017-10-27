/*
 * The ONLY global contex of seal2d, you could simply destroy everything by delete the context.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_file_system.h"
#include "s2d_sprite_renderer.h"
#include "s2d_camera.h"
#include "s2d_sprite.h"
#include "s2d_bmfont.h"

#ifdef S2D_ENABLE_LUA
#include "s2d_lua_context.h"
#endif

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

    void on_touch_begin(float x, float y);
    void on_touch_moved(float x, float y);
    void on_touch_ended(float x, float y);
    void on_touch_cancl(float x, float y);

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
    app_protocol*       _app;
    touch_handler*      _touch_handler;
    file_system*        _file_system;
    sprite_frame_cache* _sprite_frame_cache;
    bmfont_info_cache*  _bmfont_info_cache;
    texture_cache*      _texture_cache;
    sprite_renderer*    _sprite_renderer;
    node*               _root;
    camera*             _camera;
#ifdef S2D_ENABLE_LUA
    lua_context*        _lua_context;
#endif

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

#endif
