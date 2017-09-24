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

#include "s2d_gl_util.h"
#include "s2d_util.h"

NS_S2D

uint64_t gl_util::_gl_cap_bits = 0;
uint32_t gl_util::_gl_version = 3;

void gl_util::check_gl_error(const char* file, int line)
{
    GLenum err = GL_NO_ERROR;
    if((err = glGetError()) != GL_NO_ERROR) {
        LOGE("gl error: %04x. file = %s, line = %d\n", err, file, line);
    }
}

void gl_util::check_gl_extension(uint32_t glversion)
{
    _gl_version = glversion;

    if (glversion == 3) {
        _gl_cap_bits |= GL_CAP_SUPPORT_VAO;
    } else {
        LOGE("not check opengles 2.0 yet.");
        S2D_ASSERT(false);
    }

    const char* gl_vendor = (const char*)glGetString(GL_VENDOR);
    const char* gl_renderer = (const char*)glGetString(GL_RENDERER);
    const char* gl_version = (const char*)glGetString(GL_VERSION);
    const char* gl_extension = (const char *)glGetString(GL_EXTENSIONS);

    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);

    for (int i = 0; i < n; ++i) {
        LOGD("%d: %s", i, (const char*)glGetStringi(GL_EXTENSIONS, i));
    }

    LOGD("gl_vendor = %s", gl_vendor);
    LOGD("gl_renderer = %s", gl_renderer);
    LOGD("gl_version = %s", gl_version);
    LOGD("gl_extension = %s", gl_extension);
    LOGD("gl: support vao = %s", support_vao() ? "true" : "false");
}

NS_S2D_END
