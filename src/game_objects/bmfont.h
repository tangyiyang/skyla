/*
 * Bitmap font for skyla, see: http://www.angelcode.com/products/bmfont/
 * You may create the font atlas with https://71squared.com/glyphdesigner
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_bmfont_h
#define skyla_bmfont_h

#include "skyla_common.h"
#include "game_objects/node.h"
#include "game_objects/sprite.h"
#include "renderer/texture.h"

NS_SKYLA

typedef uint32_t bmfont_charc_id_t;
struct bmfont_character {
    uint32_t id;     /* 4 bytes should be enough for most cases. */
    int x, y, width, height;
    int xoffset, yoffset;
    int xadvance;
    int page; /* should be 0 all the time, cause we only support 1 page right now. */
    int chnl; /* not used right now. */
    char letter[8]; /* for implemention simplicty, use more bytes :), 4 bytes is enough for most cases. */
};

class bmfont_info : public ref_counter {
public:
    struct info {
        /* we only need the spacing right now
         * char face[BM_FACE_NAME_LEN];
         * size_t size;
         * int bold;
         * int italic;
         * char charset[CHARSET_NAME_LEN];
         * int unicode;
         * int stretchH;
         * int smooth;
         * int aa;
         * int padding[4];
         */
        struct spacing {
            int x;
            int y;
        } spacing;
    } info;

    struct common {
        int lineHeight;
        int base;
        int scaleW;
        int scaleH;
        int pages;
        int packed;
    } common;

    struct page {
        int id;
        std::string file; /* this is where we store the texture. */
    } page;

public:
    std::string font_file;
    std::map<bmfont_charc_id_t, bmfont_character*> _characters;
};

class bmfont_info_cache {
public:
    bmfont_info* load_font(const char* atlas_file_path);
    void shutdown();

private:
    bmfont_info* load_font_from_buffer(const uint8_t* buffer);

private:
    std::map<std::string, bmfont_info*> _cache;
};

class bmfont : public node {
public:
    bmfont();

public:
    void init(const char* text, const char* font_atlas);
    /*
     *  bmfont doesn't need hit_test for most cases.
     *
     *  virtual void hit_test(touch_handler* handler, touch_event* event);
     *  virtual void on_touch(touch_event* event);
     */

public:
    void set_text(const char* text);

private:
    void update_text(const char* new_text);
    void append_glyph(uint32_t codepoint, float *cursor_x, float *cursor_y);

private:
    bmfont_info* _font_info; /* font metrics info */
    texture*     _texture;
    std::string  _text;
};

NS_SKYLA_END

#endif
