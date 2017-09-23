//
//  Created by yiyang.tang on 2017/9/11.
//

#include "s2d_context.h"
#include "s2d_gl_util.h"
#include "s2d_util.h"

NS_S2D

context* context::_global_context = nullptr;

void context::init(float width, float height)
{
    _global_context = this;

    util::init(_global_context);

    _sprite_renderer = new sprite_renderer();
    _file_system = new file_system();
    _camera = new camera();
    _root = new node();

    _sprite_renderer->init();
    _file_system->init();
    _camera->init_orthographic(width, height);
    _root->init();
    _root->set_pos(100, 100);
    _root->set_scale(1.5);

    _world_view_affine_transform = affine_transform::mk_translate(-width/2, -height/2);

    // TODO: move this to somewhere else after finish the basic needs of the engine.
    sprite* first = new sprite();
    first->init();
    first->set_scale(0.5);
    _root->add_child(first);
}

void context::loop(float dt)
{
    _root->update(dt);
    _sprite_renderer->flush();
}

void context::shutdown()
{
    _sprite_renderer->shutdown();
    _root->remove_all_children();
    delete _root;
}

NS_S2D_END
