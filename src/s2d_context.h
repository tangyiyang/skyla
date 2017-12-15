/*
 * The ONLY global contex of seal2d, you could simply destroy everything by delete the context.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_file_system.h"
#include "s2d_render_state.h"
#include "s2d_camera.h"
#include "s2d_sprite.h"
#include "s2d_bmfont.h"
#include "s2d_profiler.h"
#include "s2d_timer.h"

#if (S2D_ENABLE_LUA == 1)
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
    virtual void on_begin_update(float dt) = 0;
    virtual void on_after_update(float dt) = 0;

public:
    std::string _app_name;
    std::string _app_version;
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
    void update(float dt);
    void draw();
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

    inline void append_release_node(node* n)
    {
        _marked_release_nodes.push_back(n);
    }

private:
    void update_resolution_settings(float window_width, float window_height);
    void init_fundamental_components(const size& logic_size);

public:
    /* The Core Components */
    app_protocol*       _app;
    bmfont_info_cache*  _bmfont_info_cache;
    camera*             _camera;
    file_system*        _file_system;
    touch_handler*      _touch_handler;
    timer_mgr*          _timer_mgr;
    render_state*       _render_state;
    node*               _root;
    sprite_frame_cache* _sprite_frame_cache;
    texture_cache*      _texture_cache;
    profiler*           _profiler;

#if (S2D_ENABLE_LUA == 1)
    lua_context*        _lua_context;
#endif

    /* Core Coordinate System */
    affine_transform _world_view_affine_transform;
    size             _logic_size;
    size             _window_size;
    rect             _viewport_rect;
    rect             _visible_rect;
    vec2             _scale_factor;
    RESOLUTION_COMPAT_TYPE _resolution_compat_type;
    float            _content_scale_factor; // on retina screen, this may be 2 or 3.

    /* Object lifecycle management */
    std::vector<node*>  _marked_release_nodes;
    
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
