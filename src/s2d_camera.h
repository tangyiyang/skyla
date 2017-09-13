//
//  Created by yiyang.tang on 2017/9/13.
//

#ifndef s2d_camera_h
#define s2d_camera_h

#include "s2d_common.h"

NS_S2D

class camera {
public:

    void init_orthographic(float width, float height);
    void translate(const vec2& offset);

public:
    matrix4 _m;

    size _size;
};

NS_S2D_END

#endif /* s2d_camera_h */
