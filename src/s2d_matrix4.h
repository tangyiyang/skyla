//
//  Created by yiyang.tang on 2017/9/13.
//

#ifndef s2d_matrix4_h
#define s2d_matrix4_h

#include "s2d_common.h"

NS_S2D

struct matrix4 {

    float m[16];


    static matrix4* alloc_orthographic(float left, float right, float bottom, float top, float near, float far);
    static void init_orthograic(matrix4* m, float left, float right, float bottom, float top, float near, float far);
    static matrix4 translate(const matrix4* m, float tx, float ty, float tz);
    static matrix4 multiply(const matrix4* m1, const matrix4* m2);
};

NS_S2D_END

#endif /* s2d_matrix4_h */
