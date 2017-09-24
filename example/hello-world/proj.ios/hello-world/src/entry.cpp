#include "entry.h"

void entry::on_init(context* ctx)
{
    LOGD("entry:: on_init");
    sprite* s = new sprite();
    s->init("res/seal2d-transparent.png");
    s->set_pos(640/2, 1136/2);

    ctx->get_root()->add_child(s);
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
