#include "entry.h"

void entry::on_init(context* ctx)
{

    rect visible_rect = ctx->get_visible_rect();

    vec2 center = { (visible_rect.size.width - visible_rect.origin.x)/2,
                    (visible_rect.size.height - visible_rect.origin.y)/2 };
    LOGD("entry:: on_init");

    ctx->_sprite_frame_cache->load("res/animations/pirate.json", "res/animations/pirate.png");

    node* root = ctx->get_root();
    root->set_anchor(0, 0);
    root->set_pos(0, 0);

    node* first = new node();
    first->init();
    first->set_pos(0, 0);
    first->set_anchor(0, 0);

    sprite* s = new sprite();
    s->init("res/seal2d-transparent.png");
    s->set_pos(visible_rect.size.width/2, visible_rect.size.height/2);
    s->set_anchor(0.5, 0.5);
    first->add_child(s);

    root->add_child(first);
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
