/*
 * Auxiliary for seal2d
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_util_h
#define s2d_util_h

#include "s2d_common.h"
#include "s2d_texture.h"

NS_S2D

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

#define S2D_ASSERT(cond) do { \
                            if (!(cond)) { \
                                LOGD(STRINGIFY(cond)); \
                            } \
                            assert(cond); \
                            } while(0);

#define LOGD(format, ...) s2d::util::log(S2D_LOG_DEBUG, format "\n", ##__VA_ARGS__);
#define LOGE(format, ...) s2d::util::log(S2D_LOG_ERROR, format "\n", ##__VA_ARGS__);

#define LOGD_NO_NEW_LINE(format, ...) s2d::util::log(S2D_LOG_DEBUG, format, ##__VA_ARGS__);

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

class file_entry;
class util {
public:
    static uint32_t utf8_len(const char* utf8str);
    static uint32_t utf8_decode(uint32_t* _state, uint32_t* _codep, uint8_t _ch);
    
    static void log(int level, const char* format, ...);
    static file_entry* load_file(const char* path, bool cache);
    static texture* load_texture(const char* path);

    static std::string md5(file_entry* file_entry);
};

NS_S2D_END


#endif
