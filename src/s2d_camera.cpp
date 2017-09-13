//
//  Created by yiyang.tang on 2017/9/13.
//

#include "s2d_camera.h"

NS_S2D

void camera::init_orthographic(float width, float height)
{
    matrix4::init_orthograic(&this->_m, 0, width, 0, height, -1.0, 1.0);
}

void camera::translate(const vec2& offset)
{
    _m = matrix4::translate(&_m, offset.x, offset.y, 1.0);
}

NS_S2D_END
