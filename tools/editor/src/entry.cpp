#include "entry.h"

void entry::on_init(context* ctx)
{

    rect visible_rect = ctx->get_visible_rect();

    vec2 center = { (visible_rect.size.width - visible_rect.origin.x)/2,
                    (visible_rect.size.height - visible_rect.origin.y)/2 };
    LOGD("entry:: on_init");

//    ctx->_sprite_frame_cache->load("res/animations/pirate.json", "res/animations/pirate.png");



    node* root = ctx->get_root();
    root->set_content_size(visible_rect.size.width, visible_rect.size.height);
    root->set_anchor(0, 0);
    root->set_pos(100, 0);

    float scale_x = visible_rect.size.width / 16.0f;
    float scale_y = visible_rect.size.height / 16.0f;
    sprite* background = new sprite();
    background->init("res/editor_resoruces/backgroud.png");
    background->set_scale(scale_x, scale_y);
    background->set_pos(0, 0);
    background->set_anchor(0, 0);
    root->add_child(background);

    _background = background;

    sprite* s = new sprite();
    s->init("res/seal2d-transparent.png");
    s->set_pos(visible_rect.size.width/2, visible_rect.size.height/2);
    s->set_anchor(0.5, 0.5);

    root->add_child(s);

    root->set_scale(0.5);
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
    LOGD("entry:: on_resize");
//    rect visible_rect = ctx->get_visible_rect();
//    float scale_x = visible_rect.size.width / 16.0f;
//    float scale_y = visible_rect.size.height / 16.0f;
//    vec2 center = { (visible_rect.size.width - visible_rect.origin.x)/2,
//        (visible_rect.size.height - visible_rect.origin.y)/2 };
//
//    _background->set_scale(scale_x, scale_y);
//    _background->set_pos(center.x, center.y);
}
