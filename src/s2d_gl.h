/*
 * OpenGL(ES) Utility class.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_gl_util_h
#define s2d_gl_util_h

#include "s2d_common.h"
#include "s2d_platform.h"

#if S2D_IS_IOS
#include <OpenGLES/ES3/glext.h>
#elif S2D_IS_MAC
#include <OpenGL/gl3.h>
#else
#error "opengl not included."
#endif

#ifdef DEBUG
#define CHECK_GL_ERROR s2d::gl::check_error(__FILE__, __LINE__);
#else
#define CHECK_GL_ERROR do{}while(0);
#endif

NS_S2D

struct blend_func {
    GLenum src;
    GLenum dst;
};

class gl {
public:
    static void check_extension(uint32_t glversion);
    static void check_error(const char* file, int line);

private:
    static uint64_t _cap_bits;
    static uint32_t _version;
};

NS_S2D_END
#endif
