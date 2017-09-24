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

#ifndef s2d_macro_h
#define s2d_macro_h

#include <assert.h>

#define S2D_LOG_VERBOSE (0)
#define S2D_LOG_DEBUG   (1)
#define S2D_LOG_INFO    (2)
#define S2D_LOG_WARN    (3)
#define S2D_LOG_ERROR   (4)
#define S2D_LOG_FATAL   (5)

static const char* LOG_LEVEL_STR[] = {
    "V",
    "D",
    "I",
    "W",
    "E",
    "F"
};

#if (defined DEBUG) && (!defined S2D_LOG_LEVEL)
#define S2D_LOG_LEVEL S2D_LOG_DEBUG
#endif

#if (!defined RELEASE) && (!defined S2D_LOG_LEVEL)
#define S2D_LOG_LEVEL S2D_LOG_ERROR
#endif

#ifndef S2D_LOG_LEVEL
#define S2D_LOG_LEVEL (S2D_LOG_LEVEL_DEBUG)
#endif

#define STRINGIFY(s) #s

#define NS_S2D namespace s2d {
#define NS_S2D_END }

#define SAFE_RELEASE(obj) do { if(obj) { obj->release(); }}while(0);

#endif 
