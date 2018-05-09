#include "entry.h"

void entry::on_init(context* ctx)
{
    _time_elapsed = 0.0f;
    this->_app_name = "seal2d.hello-world";
    this->_app_version = "v0.0.1";

#if S2D_ENABLE_LUA == 1
    ctx->_lua_context->on_start(ctx, "res/scripts/main.lua");
#else

    rect visible_rect = ctx->get_visible_rect();
    LOGD("entry:: on_init");

    ctx->_sprite_frame_cache->load("res/animations/pirate.json", "res/animations/pirate.png");
    ctx->_sprite_frame_cache->load("res/images/ui_atlas.json", "res/images/ui_atlas.png");

    node* root = ctx->get_root();
    root->set_anchor(0, 0);
    root->set_pos(0, 0);

    primitive_node* p = new primitive_node();
    p->init();
    p->add_line(vec2::make(0, 0), vec2::make(100, 100), 0x0000ff00);
    root->add_child(p);

#endif
}

void entry::on_pause()
{
    LOGD("entry:: on_pause");
}

void entry::on_resume()
{
    LOGD("entry:: on_resume");
}

void entry::on_destroy()
{
    LOGD("entry:: on_destroy");
}

void entry::on_resize(context* ctx)
{

}

void entry::on_begin_update(float dt)
{

}

void entry::on_after_update(float dt)
{
    _time_elapsed += dt;
    if (_time_elapsed > 2.0f) {
        _time_elapsed -= 2.0f;
        util::profile_stats();
    }
}
