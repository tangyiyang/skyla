#include "renderer.h"

NS_SKYLA

blend_func blend_func::NORMAL   = { GL_ONE,          GL_ONE_MINUS_SRC_ALPHA };
blend_func blend_func::ADDITIVE = { GL_ONE,          GL_ONE                 };
blend_func blend_func::MULTIPLY = { GL_DST_COLOR,    GL_ONE_MINUS_SRC_ALPHA };
blend_func blend_func::SCREEN   = { GL_ONE,          GL_ONE_MINUS_SRC_COLOR };
blend_func blend_func::ALPHA    = { GL_SRC_ALPHA,    GL_ONE_MINUS_SRC_ALPHA };

NS_SKYLA_END
