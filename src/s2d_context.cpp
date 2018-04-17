#include "s2d_context.h"
#include "s2d_util.h"

NS_S2D

context* context::_global_context = nullptr;

void context::init(int opengles_version, float window_width, float window_height)
{
    _global_context = this;
    _logic_size = {window_width, window_height};
    _content_scale_factor = 1.0f;
    s2d::gl::check_extension(opengles_version);

    update_resolution_settings(window_width, window_height);
    init_fundamental_components(_logic_size);
    LOGD("context initialized.");

    if (_app) {
        _app->on_init(this);
    }
}

void context::update_resolution_settings(float window_width, float window_height)
{
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
    LOGD("** viewport origin = {%.2f, %.2f}", _viewport_rect.origin.x, _viewport_rect.origin.y);
    LOGD("** viewport size = {%.2f, %.2f}", _viewport_rect.size.width, _viewport_rect.size.height);
    LOGD("** logic_size size = {%.2f, %.2f}", _logic_size.width, _logic_size.height);
    LOGD("** visible origin = {%.2f, %.2f}", _visible_rect.origin.x, _visible_rect.origin.y);
    LOGD("** visible size = {%.2f, %.2f}", _visible_rect.size.width, _visible_rect.size.height);
}

void context::init_fundamental_components(const size& logic_size)
{
    _texture_cache = new texture_cache();
    _sprite_frame_cache = new sprite_frame_cache();
    _render_state = new render_state();
    _timer_mgr = new timer_mgr();
    _action_mgr = new action_mgr();
    _file_system = new file_system();
    _touch_handler = new touch_handler();
    _bmfont_info_cache = new bmfont_info_cache();
    _camera = new camera();
    _root = new node();
    _profiler = new profiler();

#if (S2D_ENABLE_LUA == 1)
    _lua_context = new lua_context();
    _lua_context->init();
#endif

    _timer_mgr->init();
    _action_mgr->init();
    _render_state->init();
    _file_system->init();
    _camera->init_orthographic(logic_size.width, logic_size.height);
    _root->init();

    _world_view_affine_transform = affine_transform::mk_translate(-logic_size.width/2,
                                                                  -logic_size.height/2);
}

void context::update(float dt)
{
    if (_app) {
        _app->on_begin_update(dt);
    }

    _timer_mgr->update(dt);
    _action_mgr->update(dt);
    _root->update(dt);

#if (S2D_ENABLE_LUA == 1)
    _lua_context->on_update(dt);
#endif

    if (!_marked_release_nodes.empty()) {
        std::vector<node*>::iterator it = _marked_release_nodes.begin();
        for (; it != _marked_release_nodes.end(); ++it) {
            delete *it;
        }
        _marked_release_nodes.clear();
    }

    if (_app) {
        _app->on_after_update(dt);
    }
}

void context::draw()
{
    _content_scale_factor = 2;
    glViewport(_viewport_rect.origin.x,
               _viewport_rect.origin.y,
               _viewport_rect.size.width * _content_scale_factor,
               _viewport_rect.size.height * _content_scale_factor);

    _render_state->clear();
    
    _root->draw(_render_state);

    _render_state->flush();
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
    _root->remove_all_children();

    _sprite_frame_cache->shutdown();
    _bmfont_info_cache->shutdown();
    _render_state->shutdown();
    _file_system->shutdown();
    _profiler->shutdown();
    _timer_mgr->shutdown();
    _action_mgr->shutdown();

    delete _touch_handler;
    delete _sprite_frame_cache;
    delete _bmfont_info_cache;
    delete _texture_cache;
    delete _render_state;
    delete _file_system;
    delete _root;
    delete _camera;
    delete _profiler;
    delete _timer_mgr;
    delete _action_mgr;

#if (S2D_ENABLE_LUA == 1)
    delete _lua_context;
#endif

    if (_app) {
        _app->on_destroy();
    }
}

void context::on_touch_begin(float x, float y)
{
    touch_event e = {
        touch_handler::touch_id(),
        x, _window_size.height - y,
        touch_event::TOUCH_BEGIN
    };
    _touch_handler->handle_touch_event(&e);
}

void context::on_touch_moved(float x, float y)
{
    touch_event e = {
        touch_handler::touch_id(),
        x, _window_size.height - y,
        touch_event::TOUCH_MOVED
    };
    _touch_handler->handle_touch_event(&e);
}

void context::on_touch_ended(float x, float y)
{
    touch_event e = {
        touch_handler::touch_id(),
        x, _window_size.height - y,
        touch_event::TOUCH_ENDED
    };
    _touch_handler->handle_touch_event(&e);
}

void context::on_touch_cancl(float x, float y)
{
    touch_event e = {
        touch_handler::touch_id(),
        x, _window_size.height - y,
        touch_event::TOUCH_CANCL
    };
    _touch_handler->handle_touch_event(&e);
}

void context::set_logic_size(float width, float height)
{
    if (FLT_EQUAL(width, _logic_size.width) && FLT_EQUAL(height, _logic_size.height)) {
        return;
    }

    _logic_size = {width, height};
    update_resolution_settings(_window_size.width, _window_size.height);
    if (_app) {
        _app->on_resize(this);
    }
}

void context::set_content_scale_factor(float factor)
{
    if (FLT_EQUAL(_content_scale_factor, factor)) {
        return;
    }

    _content_scale_factor = factor;
}

NS_S2D_END
