/*
 * Platform detect macros and some convience helper macros.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_platform_h
#define skyla_platform_h

#ifdef __ANDROID__
    #define SKYLA_TARGET_ANDROID (1)
#elif __APPLE__
    #include "TargetConditionals.h"
    #if (TARGET_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE)
        #define SKYLA_TARGET_IOS (2)
    #elif TARGET_OS_MAC
        #define SKYLA_TARGET_OSX (4)
    #else
    #   error "Unknown Apple platform"
    #endif
#elif _WIN32
    #define SKYLA_TARGET_WINDOWS (6)
#else
#   error "unknown platform"
#endif

#ifndef SKYLA_TARGET_PLATFORM
#if (defined SKYLA_TARGET_ANDROID)
    #define SKYLA_TARGET_PLATFORM (SKYLA_TARGET_ANDROID)
#elif (defined SKYLA_TARGET_IOS)
    #define SKYLA_TARGET_PLATFORM (SKYLA_TARGET_IOS)
#elif (defined SKYLA_TARGET_OSX)
    #define SKYLA_TARGET_PLATFORM (SKYLA_TARGET_OSX)
#else
#    error "unsupport platform"
#endif

#define SKYLA_IS_IOS (SKYLA_TARGET_PLATFORM == SKYLA_TARGET_IOS)
#define SKYLA_IS_MAC (SKYLA_TARGET_PLATFORM == SKYLA_TARGET_OSX)
#define SKYLA_IS_WINDOWS (SKYLA_TARGET_PLATFORM == SKYLA_TARGET_WINDOWS)
#define SKYLA_IS_APPLE (SKYLA_IS_IOS || SKYLA_IS_MAC)
#define SKYLA_IS_ANDROID (SKYLA_TARGET_PLATFORM == SKYLA_TARGET_ANDROID)
#define SKYLA_IS_MOBILE (SKYLA_IS_IOS || SKYLA_IS_ANDROID)
#define SKYLA_IS_DESKTOP (SKYLA_IS_MAC || SKYLA_IS_WINDOWS)

#endif

#endif
