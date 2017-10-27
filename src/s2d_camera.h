/*
 * Camera full implemention is on it's way.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_camera_h
#define s2d_camera_h

#include "s2d_common.h"
#include "s2d_math.h"

NS_S2D

class camera {
public:
    void init_orthographic(float width, float height);
    void translate(const vec2& offset);

public:
    matrix3 _matrix;

    size _size;
};

NS_S2D_END

#endif
