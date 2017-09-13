//
//  Created by yiyang.tang on 2017/9/13.
//

#ifndef s2d_matrix4_h
#define s2d_matrix4_h

#include "s2d_macro.h"
#include "s2d_math.h"

NS_S2D

struct matrix4 {

    float m[16];


    static matrix4* alloc_orthographic(float left, float right, float bottom, float top, float near, float far);
    static void init_orthograic(matrix4* m, float left, float right, float bottom, float top, float near, float far);
    static void init_translate(matrix4* m, float x, float y, float z);

    static matrix4 translate(const matrix4* m, float tx, float ty, float tz);
    static matrix4 multiply(const matrix4* m1, const matrix4* m2);
    static vec2 multiply_vec2(const matrix4* m, const vec2& v);
    static vec2 multiply_vec2_trans(const matrix4* m, const vec2& v);
    static vec3 multiply_vec3(const matrix4* m, const vec3& v);
    static vec4 multiply_vec4(const matrix4* m, const vec4& v);

};

NS_S2D_END

#endif /* s2d_matrix4_h */
