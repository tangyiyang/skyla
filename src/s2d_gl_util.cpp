//
//  Created by yiyang.tang on 2017/9/11.
//

#include "s2d_gl_util.h"

void s2d_gl_util::check_gl_extension()
{
    const char* gl_vendor = (const char*)glGetString(GL_VENDOR);
    const char* gl_renderer = (const char*)glGetString(GL_RENDERER);
    const char* gl_version = (const char*)glGetString(GL_VERSION);
    const char* gl_extension = (const char *)glGetString(GL_EXTENSIONS);

    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);

    for (int i = 0; i < n; ++i) {
        printf("%d: %s\n", i, (const char*)glGetStringi(GL_EXTENSIONS, i));
    }

    printf("gl_vendor = %s\n", gl_vendor);
    printf("gl_renderer = %s\n", gl_renderer);
    printf("gl_version = %s\n", gl_version);
    printf("gl_extension = %s\n", gl_extension);
}
