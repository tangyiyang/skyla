//
//  Created by yiyang.tang on 2017/9/11.
//

#include "s2d_context.h"

NS_S2D
void s2d_context::init()
{
    s2d_gl_util::check_gl_extension();

    _root = new s2d::sprite();
    _root->init();
}

void s2d_context::update()
{
    _root->update();
}
void s2d_context::draw()
{

}

void s2d_context::shutdown()
{
    
}

NS_S2D_END
