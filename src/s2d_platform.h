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

#ifndef s2d_platform_h
#define s2d_platform_h

#ifdef __ANDROID__
    #define S2D_TARGET_ANDROID (1)
#elif __APPLE__
    #include "TargetConditionals.h"
    #if (TARGET_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE)
        #define S2D_TARGET_IOS (2)
    #elif TARGET_OS_MAC
        #define S2D_TARGET_OSX (4)
    #else
    #   error "Unknown Apple platform"
    #endif
#elif _WIN32
    #define S2D_TARGET_WINDOWS (6)
#else
#   error "unknown platform"
#endif

#ifndef S2D_TARGET_PLATFORM
#if (defined S2D_TARGET_ANDROID)
    #define S2D_TARGET_PLATFORM (S2D_TARGET_ANDROID)
#elif (defined S2D_TARGET_IOS)
    #define S2D_TARGET_PLATFORM (S2D_TARGET_IOS)
#elif (defined S2D_TARGET_OSX)
    #define S2D_TARGET_PLATFORM (S2D_TARGET_OSX)
#else
#    error "unsupport platform"
#endif

#define S2D_IS_IOS (S2D_TARGET_PLATFORM == S2D_TARGET_IOS)
#define S2D_IS_MAC (S2D_TARGET_PLATFORM == S2D_TARGET_OSX)
#define S2D_IS_APPLE (S2D_IS_IOS || S2D_IS_MAC)

#endif

#endif /* s2d_platform_h */
