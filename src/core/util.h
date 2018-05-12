/*
 * Auxiliary for skyla
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef util_h
#define util_h

#include "skyla_common.h"
#include "renderer/texture.h"

NS_SKYLA

#define SKYLA_LOG_VERBOSE (0)
#define SKYLA_LOG_DEBUG   (1)
#define SKYLA_LOG_INFO    (2)
#define SKYLA_LOG_WARN    (3)
#define SKYLA_LOG_ERROR   (4)
#define SKYLA_LOG_FATAL   (5)

static const char* LOG_LEVEL_STR[] = {
    "V",
    "D",
    "I",
    "W",
    "E",
    "F"
};

#if (defined DEBUG) && (!defined SKYLA_LOG_LEVEL)
#define SKYLA_LOG_LEVEL SKYLA_LOG_DEBUG
#endif

#if (!defined RELEASE) && (!defined SKYLA_LOG_LEVEL)
#define SKYLA_LOG_LEVEL SKYLA_LOG_ERROR
#endif

#ifndef SKYLA_LOG_LEVEL
#define SKYLA_LOG_LEVEL (SKYLA_LOG_LEVEL_DEBUG)
#endif

#define SKYLA_ASSERT(cond) do { if (!(cond)) { LOGD(STRINGIFY(cond)); } assert(cond); } while(0);

#define LOGD(format, ...) util::log(SKYLA_LOG_DEBUG, format "\n", ##__VA_ARGS__);
#define LOGE(format, ...) util::log(SKYLA_LOG_ERROR, format "\n", ##__VA_ARGS__);

#define LOGD_NO_NEW_LINE(format, ...) util::log(SKYLA_LOG_DEBUG, format, ##__VA_ARGS__);

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

class file_entry;
class util {
public:
    static uint32_t     utf8_len(const char* utf8str);
    static uint32_t     utf8_decode(uint32_t* _state, uint32_t* _codep, uint8_t _ch);

    static void         log(int level, const char* format, ...);

    static void         insert_search_path(const char* full_path);
    static file_entry*  load_file(const char* path, bool cache);
    static texture*     load_texture(const char* path);

    static void         profile_begin(const char* name);
    static void         profile_end(const char* name);
    static void         profile_stats();

    static std::string  md5(file_entry* file_entry);

    static inline float normalized_random()
    {
        return -1.0f + (((double)rand())/RAND_MAX) * (1.0 - (-1.0));
    }

    static const char* get_platform();
};

NS_SKYLA_END

#endif /* util_h */
