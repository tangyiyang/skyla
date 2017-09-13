//
//  Created by yiyang.tang on 2017/9/13.
//

#include "s2d_matrix4.h"
#include <stdio.h>
NS_S2D

matrix4* matrix4::alloc_orthographic(float left, float right, float bottom, float top, float near, float far)
{
    matrix4* matrix = new matrix4();
    init_orthograic(matrix, left, right, bottom, top, near, far);
    return matrix;
}

void matrix4::init_orthograic(matrix4* matrix, float left, float right, float bottom, float top, float near, float far)
{
    float* m = matrix->m;

    m[0] = 2 / (right - left);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = 2 / (top - bottom);
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = 2 / (near - far);
    m[11] = 0;
    m[12] = (left + right) / (left - right);
    m[13] = (top + bottom) / (bottom - top);
    m[14] = (near + far) / (near - far);
    m[15] = 1;
}

void matrix4::init_translate(matrix4* matrix, float x, float y, float z)
{
    float* m = matrix->m;

    m[0] = 1;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;
    m[4] = 0;
    m[5] = 1;
    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
    m[9] = 0;
    m[10] = 1;
    m[11] = 0;
    m[12] = x;
    m[13] = y;
    m[14] = z;
    m[15] = 1;
}

matrix4 matrix4::translate(const matrix4* m, float tx, float ty, float tz)
{
    matrix4 mm = {     m->m[0], m->m[1], m->m[2], m->m[3],
                       m->m[4], m->m[5], m->m[6], m->m[7],
                       m->m[8], m->m[9], m->m[10], m->m[11],
                       m->m[0] * tx + m->m[4] * ty + m->m[8] * tz + m->m[12],
                       m->m[1] * tx + m->m[5] * ty + m->m[9] * tz + m->m[13],
                       m->m[2] * tx + m->m[6] * ty + m->m[10] * tz + m->m[14],
                       m->m[3] * tx + m->m[7] * ty + m->m[11] * tz + m->m[15]
    };
    return mm;
}

matrix4 matrix4::multiply(const matrix4* m1, const matrix4* m2)
{
    matrix4 m;

    m.m[0]  = m1->m[0] * m2->m[0]  + m1->m[4] * m2->m[1]  + m1->m[8] * m2->m[2]   + m1->m[12] * m2->m[3];
    m.m[4]  = m1->m[0] * m2->m[4]  + m1->m[4] * m2->m[5]  + m1->m[8] * m2->m[6]   + m1->m[12] * m2->m[7];
    m.m[8]  = m1->m[0] * m2->m[8]  + m1->m[4] * m2->m[9]  + m1->m[8] * m2->m[10]  + m1->m[12] * m2->m[11];
    m.m[12] = m1->m[0] * m2->m[12] + m1->m[4] * m2->m[13] + m1->m[8] * m2->m[14]  + m1->m[12] * m2->m[15];

    m.m[1]  = m1->m[1] * m2->m[0]  + m1->m[5] * m2->m[1]  + m1->m[9] * m2->m[2]   + m1->m[13] * m2->m[3];
    m.m[5]  = m1->m[1] * m2->m[4]  + m1->m[5] * m2->m[5]  + m1->m[9] * m2->m[6]   + m1->m[13] * m2->m[7];
    m.m[9]  = m1->m[1] * m2->m[8]  + m1->m[5] * m2->m[9]  + m1->m[9] * m2->m[10]  + m1->m[13] * m2->m[11];
    m.m[13] = m1->m[1] * m2->m[12] + m1->m[5] * m2->m[13] + m1->m[9] * m2->m[14]  + m1->m[13] * m2->m[15];

    m.m[2]  = m1->m[2] * m2->m[0]  + m1->m[6] * m2->m[1]  + m1->m[10] * m2->m[2]  + m1->m[14] * m2->m[3];
    m.m[6]  = m1->m[2] * m2->m[4]  + m1->m[6] * m2->m[5]  + m1->m[10] * m2->m[6]  + m1->m[14] * m2->m[7];
    m.m[10] = m1->m[2] * m2->m[8]  + m1->m[6] * m2->m[9]  + m1->m[10] * m2->m[10] + m1->m[14] * m2->m[11];
    m.m[14] = m1->m[2] * m2->m[12] + m1->m[6] * m2->m[13] + m1->m[10] * m2->m[14] + m1->m[14] * m2->m[15];

    m.m[3]  = m1->m[3] * m2->m[0]  + m1->m[7] * m2->m[1]  + m1->m[11] * m2->m[2]  + m1->m[15] * m2->m[3];
    m.m[7]  = m1->m[3] * m2->m[4]  + m1->m[7] * m2->m[5]  + m1->m[11] * m2->m[6]  + m1->m[15] * m2->m[7];
    m.m[11] = m1->m[3] * m2->m[8]  + m1->m[7] * m2->m[9]  + m1->m[11] * m2->m[10] + m1->m[15] * m2->m[11];
    m.m[15] = m1->m[3] * m2->m[12] + m1->m[7] * m2->m[13] + m1->m[11] * m2->m[14] + m1->m[15] * m2->m[15];

    return m;
}

vec2 matrix4::multiply_vec2(const matrix4* m, const vec2& v)
{
    vec4 tmp = {v.x, v.y, 1.0, 0.0};
    vec4 v4 = multiply_vec4(m, tmp);
    vec2 v2 = {v4.x, v4.y};
    return v2;
}

vec2 matrix4::multiply_vec2_trans(const matrix4* m, const vec2& v)
{
    vec4 tmp = {v.x, v.y, 1.0, 1.0};
    vec4 v4 = multiply_vec4(m, tmp);
    vec2 v2 = {v4.x, v4.y};
    return v2;
}

vec3 matrix4::multiply_vec3(const matrix4* m, const vec3& v)
{
    vec4 tmp = {v.x, v.y, v.z, 1};
    vec4 v4 = multiply_vec4(m, tmp);
    vec3 v3 = {v4.x, v4.y, v4.z};
    return v3;
}

vec4 matrix4::multiply_vec4(const matrix4* m, const vec4& v)
{
    vec4 v4 = { m->m[0] * v.x + m->m[4] * v.y + m->m[8] * v.z + m->m[12] * v.w,
                m->m[1] * v.x + m->m[5] * v.y + m->m[9] * v.z + m->m[13] * v.w,
                m->m[2] * v.x + m->m[6] * v.y + m->m[10] * v.z + m->m[14] * v.w,
                m->m[3] * v.x + m->m[7] * v.y + m->m[11] * v.z + m->m[15] * v.w };

    return v4;
}

NS_S2D_END
