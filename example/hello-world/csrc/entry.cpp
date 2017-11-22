#include "entry.h"

void entry::on_init(context* ctx)
{
#if S2D_ENABLE_LUA == 1

    ctx->_lua_context->on_start(ctx, "res/scripts/main.lua");
#else
    rect visible_rect = ctx->get_visible_rect();

    LOGD("entry:: on_init");

    ctx->_sprite_frame_cache->load("res/animations/pirate.json", "res/animations/pirate.png");

    node* root = ctx->get_root();

#if 1
//
//    int j = 0;
//    for (int i = 0; i < 40; ++i) {
//        particle* p = new particle();
//        p->init("res/particles/flower.json");
//        p->set_pos(((i+1)%10) * 100, j*100);
//        if (i%10 == 0) {
//            j++;
//        }
//        root->add_child(p);
//    }

    spine_anim* s = new spine_anim();
    s->init("res/spines/spineboy-pro.json", "res/spines/spineboy.atlas");

#else
    root->set_anchor(0, 0);
    root->set_pos(0, 0);

    panel* first = new panel();
    root->add_child(first);

    first->init(rect::make_rect(0, 0, 500, 500));
    first->set_pos(0, 0);
    first->set_anchor(0, 0);

    const char* tests[] = {"attack_0.png", "attack_1.png",
                        "attack_2.png", "attack_3.png",
                        "attack_4.png"};

    for (int i = 0; i < sizeof(tests)/sizeof(const char*); ++i) {
        sprite_frame* f = ctx->_sprite_frame_cache->get(tests[i]);
        sprite* s = new sprite();
        s->init(f);
        s->set_pos( 0 + i*120, 0);
        s->set_anchor(0, 0);
        first->add_child(s);
    }
#endif

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
