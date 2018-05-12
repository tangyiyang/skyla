/*
 * OpenGL(ES) Utility class.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_gl_util_h
#define skyla_gl_util_h

#include "skyla_common.h"
#include "platform.h"

#if SKYLA_IS_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
//#elif SKYLA_IS_MAC
//#include <OpenGL/gl3.h>
//#else
//#error "opengl not included."
#endif

#ifdef DEBUG
#define CHECK_GL_ERROR gl::check_error(__FILE__, __LINE__);
#else
#define CHECK_GL_ERROR do{}while(0);
#endif

NS_SKYLA

class gl {
public:
    static void check_extension(uint32_t glversion);
    static void check_error(const char* file, int line);

private:
    static uint64_t _cap_bits;
    static uint32_t _version;
};

NS_SKYLA_END
#endif
