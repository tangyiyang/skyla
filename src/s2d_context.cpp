//
//  Created by yiyang.tang on 2017/9/11.
//

#include "s2d_context.h"

NS_S2D

context* context::_global_context = nullptr;

void context::init(float width, float height)
{
    _global_context = this;

    s2d_gl_util::check_gl_extension();

    _root = new sprite();
    _root->init();

    _camera = new camera();
    _camera->init_orthographic(width, height);

    _model_view_affine_transform = affine_transform::mk_translate(-width/2, -height/2);
}

void context::update()
{
    _root->update();
}
void context::draw()
{
    _root->draw();
}

void context::shutdown()
{
    
}

NS_S2D_END
