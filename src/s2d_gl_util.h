//
//  Created by yiyang.tang on 2017/9/11.
//

#ifndef s2d_gl_util_h
#define s2d_gl_util_h

#include "s2d_common.h"
#include "s2d_platform.h"

#if S2D_IS_IOS
#include <OpenGLES/ES3/glext.h>
#endif

#define CHECK_GL_ERROR s2d_gl_util::check_gl_error(__FILE__, __LINE__);


class s2d_gl_util {
public:
    static void check_gl_extension();
    static void check_gl_error(const char* file, int line);

};

#endif /* s2d_gl_util_h */
