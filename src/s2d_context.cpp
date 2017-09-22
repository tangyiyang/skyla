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
    _root = new sprite();

    _sprite_renderer->init();
    _file_system->init();
    _camera->init_orthographic(width, height);
    _root->init();

    _world_view_affine_transform = affine_transform::mk_translate(-width/2, -height/2);
}

void context::update()
{
    _root->update();
}
void context::draw()
{
    _sprite_renderer->flush();
}

void context::shutdown()
{

}

NS_S2D_END
