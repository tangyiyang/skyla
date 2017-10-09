/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

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
#define CHECK_GL_ERROR s2d::gl_util::check_gl_error(__FILE__, __LINE__);
#else
#define CHECK_GL_ERROR do{}while(0);
#endif

NS_S2D

class gl_util {
public:
    static void check_gl_extension(uint32_t glversion);
    static void check_gl_error(const char* file, int line);

private:
    static uint64_t _gl_cap_bits;
    static uint32_t _gl_version;
};

NS_S2D_END
#endif
