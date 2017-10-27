/*
 * Platform detect macros and some convience helper macros.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

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
#define S2D_IS_WINDOWS (S2D_TARGET_PLATFORM == S2D_TARGET_WINDOWS)
#define S2D_IS_APPLE (S2D_IS_IOS || S2D_IS_MAC)
#define S2D_IS_ANDROID (S2D_TARGET_PLATFORM == S2D_TARGET_ANDROID)
#define S2D_IS_MOBILE (S2D_IS_IOS || S2D_IS_ANDROID)
#define S2D_IS_DESKTOP (S2D_IS_APPLE || S2D_IS_WINDOWS)

#endif

#endif
