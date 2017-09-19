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

#ifndef s2d_util_h
#define s2d_util_h

#include "s2d_common.h"

NS_S2D

#define S2D_ASSERT(cond) do { \
                            if (!(cond)) { \
                                LOGD(STRINGIFY(cond)); \
                            } \
                            assert(cond); \
                            } while(0);

#define LOGD(format, ...) s2d::util::log(S2D_LOG_DEBUG, format "\n", ##__VA_ARGS__);
#define LOGE(format, ...) s2d::util::log(S2D_LOG_ERROR, format "\n", ##__VA_ARGS__);

class context;
class file_entry;
class util {
public:
    static void         init(context* ctx) { _global_context = ctx; }
    
public:
    static void         log(int level, const char* format, ...);
    static file_entry*  load_file(const char* path);
    
private:
    static context* _global_context;
};

NS_S2D_END


#endif /* s2d_util_h */
