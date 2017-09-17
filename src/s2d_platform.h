//
//  s2d_platform.h
//  hello-world
//
//  Created by 艺洋 唐 on 17/09/2017.
//  Copyright © 2017 levelmax. All rights reserved.
//

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
#else
#    error "unsupport platform"
#endif

#define S2D_IS_IOS (S2D_TARGET_PLATFORM == S2D_TARGET_IOS)
#define S2D_IS_MAC (S2D_TARGET_PLATFORM == S2D_TARGET_OSX)
#define S2D_IS_APPLE (S2D_IS_IOS || S2D_IS_MAC)

#endif

#endif /* s2d_platform_h */
