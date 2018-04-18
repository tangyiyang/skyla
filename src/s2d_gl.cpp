#include "s2d_gl.h"
#include "s2d_util.h"

NS_S2D

uint64_t gl::_cap_bits = 0;
uint32_t gl::_version = 3;

void gl::check_error(const char* file, int line)
{
    GLenum err = GL_NO_ERROR;
    if((err = glGetError()) != GL_NO_ERROR) {
        LOGE("gl error: 0x%04x. file = %s, line = %d\n", err, file, line);
        assert(false);
    }
}

void gl::check_extension(uint32_t glversion)
{
    _version = glversion;

    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);

    float line_width[2] = {0, 0};
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, line_width);
    LOGD("gl line width range: {%.2f, %.2f}", line_width[0], line_width[1]);
    LOGD("gl_extensions:");
    for (int i = 0; i < n; ++i) {
        LOGD("%d: %s", i, (const char*)glGetStringi(GL_EXTENSIONS, i));
    }
    
    const char* gl_vendor = (const char*)glGetString(GL_VENDOR);
    const char* gl_renderer = (const char*)glGetString(GL_RENDERER);
    const char* gl_version = (const char*)glGetString(GL_VERSION);
    LOGD("gl_vendor = %s", gl_vendor);
    LOGD("gl_renderer = %s", gl_renderer);
    LOGD("gl_version = %s", gl_version);
    
#if S2D_IS_MOBILE
    const char* gl_extension = (const char *)glGetString(GL_EXTENSIONS);
    LOGD("gl_extension = %s", gl_extension);
#endif

}

NS_S2D_END
