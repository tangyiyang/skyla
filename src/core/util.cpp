#include "util.h"

#include "md5.h"

#include "platform.h"
#include "skyla_context.h"

#include <zlib.h> /*It looks like every platform has libz, all we need is to link with -lz.*/

NS_SKYLA

static const uint8_t s_utf8d[364] =
{
    // The first part of the table maps bytes to character classes that
    // to reduce the size of the transition table and create bitmasks.
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

    // The second part is a transition table that maps a combination
    // of a state of the automaton and a character class to a state.
    0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
    12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
    12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
    12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
    12,36,12,12,12,12,12,12,12,12,12,12
};

uint32_t util::utf8_len(const char* utf8str)
{
    uint32_t state = 0;
    uint32_t codepoint = 0;

    const char* p = utf8str;
    size_t i = 0;
    size_t len = strlen(p);

    uint32_t utf8len = 0;
    for (; i < len; ++i) {
        if (util::utf8_decode(&state, (uint32_t*)&codepoint, p[i]) == UTF8_ACCEPT) {
            utf8len++;
        }
    }
    return utf8len;
}

uint32_t util::utf8_decode(uint32_t* _state, uint32_t* _codep, uint8_t _ch)
{
    uint32_t byte = _ch;
    uint32_t type = s_utf8d[byte];

    *_codep = (*_state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*_codep << 6) :
    (0xff >> type) & (byte);

    *_state = s_utf8d[256 + *_state + type];
    return *_state;
}

void util::log(int level, const char* format, ...)
{
    if (level >= SKYLA_LOG_LEVEL) {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
}

void util::insert_search_path(const char* full_path)
{
    /*
     *  subtract the filename of the atlas_file_path
     *  "res/fonts/animated.fnt" -> "res/fonts/"
     */
    int i = (int)strlen(full_path);
    for (; i >= 0; --i) {
        if (full_path[i] == '/') {
            break;
        }
    }

    std::string s = std::string(full_path, i+1);
    context::C()->_file_system->insert_full_search_path(s.c_str());
}

file_entry* util::load_file(const char* path, bool cache)
{
    return context::C()->_file_system->read(path, cache);
}

texture* util::load_texture(const char* path)
{
    return context::C()->_texture_cache->load(path);
}

void util::profile_begin(const char* name)
{
    return context::C()->_profiler->begin(name);
}

void util::profile_end(const char* name)
{
    return context::C()->_profiler->end(name);
}

void util::profile_stats()
{
    return context::C()->_profiler->log_all_stats();
}

std::string util::md5(file_entry* file_entry)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);

    MD5_Update(&ctx, file_entry->_buffer, file_entry->_size);

    unsigned char result[32];
    MD5_Final(result, &ctx);
    return std::string((const char*)result, 32);
}

const char* util::get_platform()
{
#if SKYLA_IS_IOS
    return "iOS";
#elif SKYLA_IS_ANDROID
    return "Android";
#elif SKYLA_IS_MAC
    return "Darwin";
#else
    return "";
#endif
}
NS_SKYLA_END
