//
//  Created by yiyang.tang on 2017/9/13.
//

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

#define S2D_ASSERT(cond)      do { \
                                if (!(cond)) { \
                                    S2D_LOG(STRINGIFY(cond)); \
                                } \
                                assert(cond); \
                              } while(0);

#define SAFE_RELEASE(obj) do { if(obj) { obj->release(); }}while(0);

#endif 
