/*
 * Camera full implemention is on it's way.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_camera_h
#define skyla_camera_h

#include "skyla_common.h"
#include "core/math.h"

NS_SKYLA

class camera {
public:
    void init_orthographic(float width, float height);
    void translate(const vec2& offset);

public:
    matrix3 _matrix;

    size _size;
};

NS_SKYLA_END

#endif
