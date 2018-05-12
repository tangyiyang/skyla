/*
 * The ONLY global contex of skyla, you could simply destroy everything by delete the context.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_context_h
#define skyla_context_h

#include "skyla_common.h"
#include "core/camera.h"
#include "core/profiler.h"
#include "core/timer.h"
#include "game_objects/action.h"
#include "game_objects/bmfont.h"
#include "game_objects/sprite.h"
#include "platform/file_system.h"
#include "renderer/render_state.h"

#if (SKYLA_ENABLE_LUA == 1)
#include "lua_binding/lua_context.h"
#endif

NS_SKYLA

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
    void set_content_scale_factor(float factor);

    inline void set_viewport_rect(const rect& r)
    {
        _viewport_rect = r;
    }

    inline float get_content_scale_factor()
    {
        return _content_scale_factor;
    }

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
    app_protocol*           _app;
    bmfont_info_cache*      _bmfont_info_cache;
    camera*                 _camera;
    file_system*            _file_system;
    touch_handler*          _touch_handler;
    timer_mgr*              _timer_mgr;
    action_mgr*             _action_mgr;
    render_state*           _render_state;
    node*                   _root;
    sprite_frame_cache*     _sprite_frame_cache;
    texture_cache*          _texture_cache;
    profiler*               _profiler;

#if (SKYLA_ENABLE_LUA == 1)
    lua_context*            _lua_context;
#endif

    /* Core Coordinate System */
    affine_transform        _world_view_affine_transform;
    size                    _logic_size;
    size                    _window_size;
    rect                    _viewport_rect;
    rect                    _visible_rect;
    vec2                    _scale_factor;
    RESOLUTION_COMPAT_TYPE  _resolution_compat_type;
    float                   _content_scale_factor; // on retina screen, this may be 2 or 3.

    /* Object lifecycle management */
    std::vector<node*>      _marked_release_nodes;
public:
    static inline context* C()
    {
        return _global_context;
    }
private:
    /* The ONLY static context in skyla. */
    static context* _global_context;
};

NS_SKYLA_END

#endif
