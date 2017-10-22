#include "entry.h"

void entry::on_init(context* ctx)
{
//    rect visible_rect = ctx->get_visible_rect();
//    node* root = ctx->get_root();
//    root->set_size(visible_rect.size.width, visible_rect.size.height);
//    root->set_anchor(0, 0);
//    root->set_pos(0, 0);
//
//    float yoffset = 150;
//    node* layer = new node();
//    layer->init();
//    layer->set_pos(visible_rect.size.width/2, visible_rect.size.height/2 + yoffset);
//    layer->set_anchor(0.5, 0.5);
//    layer->set_size(visible_rect.size.width, visible_rect.size.height);
//    layer->set_scale(0.5);
//
//    float scale_x = visible_rect.size.width / 16.0f;
//    float scale_y = visible_rect.size.height / 16.0f;
//    sprite* background = new sprite();
//    background->init("res/editor_resoruces/backgroud.png");
//    background->set_scale(scale_x, scale_y);
//    background->set_pos(visible_rect.size.width/2, visible_rect.size.height/2);
//    background->set_anchor(0.5, 0.5);
//
//    sprite* s = new sprite();
//    s->init("res/seal2d-transparent.png");
//    s->set_pos(visible_rect.size.width/2, visible_rect.size.height/2);
//    s->set_anchor(0.5, 0.5);
//
//    root->add_child(layer);
//    layer->add_child(background);
//    layer->add_child(s);
//
//    render_texture* rt = new render_texture();
//    rt->init(visible_rect.size.width, visible_rect.size.height);
//    rt->draw(root);
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
}
