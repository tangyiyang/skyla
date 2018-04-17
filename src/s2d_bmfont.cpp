#include "s2d_context.h"
#include "s2d_bmfont.h"

NS_S2D

static int parse_int(char* data)
{
    int n = 0;
    char* pos = strchr(data, '='); // go through the key

    sscanf(pos+1, "%d", &n);
    return n;
}

static void parse_str(char* data, char* val)
{
    char* pos = strchr(data, '=');
    sscanf(pos+1, "%s", val);
}

bmfont_info* bmfont_info_cache::load_font_from_buffer(const uint8_t* buffer)
{
    bmfont_info* font = new bmfont_info();
    char dummy[256] = "";
    char dummy_8[32] = "";
    char space[64] = "";
    char* line = strtok((char*)buffer, "\n");
    char tag[32] = "";
    while(line) {
        memset(tag, 0, 32);
        sscanf(line, "%s", tag);
        if (!strcmp(tag, "info")) {
            sscanf(line, "%s %s %s %s %s %s %s %s %s %s %s %s",
                   tag, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, space);
            char space_data[16] = "";
            parse_str(space, space_data);
            sscanf(space_data, "%d,%d", &font->info.spacing.x, &font->info.spacing.y);
        } else if (!strcmp(tag, "common")) {
            char lineHeight[32] = "";
            char base[32] = "";
            char scaleW[32] = "";
            char scaleH[32] = "";
            char pages[32] = "";
            char packed[32] = "";
            sscanf(line, "%s %s %s %s %s %s %s", tag, lineHeight, base, scaleW, scaleH, pages, packed);

            font->common.lineHeight = parse_int(lineHeight);
            font->common.base = parse_int(base);
            font->common.scaleW = parse_int(scaleW);
            font->common.scaleH = parse_int(scaleH);
            font->common.pages = parse_int(pages);
            font->common.packed = parse_int(packed);
        } else if (!strcmp(tag, "page")) {
            char page[32] = "";
            char file[128] = "";
            sscanf(line, "%s %s %s", tag, page, file);
            font->page.id = parse_int(page);
            char tmp[128] = "";
            parse_str(file, tmp);
            /* remove the quotes. \"animated.png\" -> animated.png */
            font->page.file = std::string(tmp+1, strlen(tmp)-2);

        } else if (!strcmp(tag, "chars")) {
            char count[16] = "";
            sscanf(line, "%s %s", tag, count);
            int char_count = parse_int(count);

            for (int i = 0; i < char_count; ++i) {
                line = strtok(NULL, "\n");
                bmfont_character* c = new bmfont_character();

                sscanf(line, "char%[ ]id=%d%[ ]x=%d%[ ]y=%d%[ ]width=%d%[ ]"
                       "height=%d%[ ]xoffset=%d%[ ]yoffset=%d%[ ]xadvance=%d%[ ]"
                       "page=%d%[ ]chnl=%d%[ ]letter=\"%[^\n]"
                       , dummy_8 , &c->id
                       , dummy_8 , &c->x         , dummy_8 , &c->y
                       , dummy_8 , &c->width     , dummy_8 , &c->height
                       , dummy_8 , &c->xoffset   , dummy_8 , &c->yoffset
                       , dummy_8 , &c->xadvance  , dummy_8 , &c->page
                       , dummy_8 , &c->chnl      , dummy_8 , c->letter);
                if (c->id == 32) { // space for special case
                    c->letter[0] = ' ';
                    c->letter[1] = 0;
                } else {
                    size_t len = strlen(c->letter);
#ifdef S2D_IS_WINDOWS
                    if (c->letter[len - 1] == '\r') len--;
#endif
                    c->letter[len - 1] = 0;
                }
                font->_characters[c->id] = c;
            }
            break; //finished reading
        }
        line = strtok(NULL, "\n");
    }
    return font;
}

bmfont_info* bmfont_info_cache::load_font(const char *atlas_file_path)
{
    std::map<std::string, bmfont_info*>::iterator found = _cache.find(atlas_file_path);
    if (found != _cache.end()) {
        return found->second;
    } else {
        file_entry* f = util::load_file(atlas_file_path, false);
        if (!f) {
            LOGE("load font atlas file failed, path = %s", atlas_file_path);
            return nullptr;
        }

        /* due to the .fnt file format only contains the relative path of the texture file.
         * so we must add the relative path the to the `search_path`, so that we could be able to
         * load the texture later on.
         */
        util::insert_search_path(f->_path.c_str());

        bmfont_info* font_info = load_font_from_buffer(f->_buffer);
        f->release();
        _cache[atlas_file_path] = font_info;
        return font_info;
    }
}

void bmfont_info_cache::shutdown()
{
    std::map<std::string, bmfont_info*>::iterator it = _cache.begin();
    for (; it != _cache.end(); ++it) {
        bmfont_info* font = it->second;
        font->release();
    }
}

bmfont::bmfont()
{
    _font_info = nullptr;
    _texture = nullptr;
}

void bmfont::init(const char* text, const char* font_atlas)
{
    node::init();
    context* c = context::C();
    _font_info = c->_bmfont_info_cache->load_font(font_atlas);
    _texture = c->_texture_cache->load(_font_info->page.file.c_str());

    this->set_text(text);
}

void bmfont::set_text(const char* text)
{
    /* if text is null or empty string, we simply clean up the vertex. */
    if (text == nullptr || (text != nullptr && strlen(text) == 0)) {
        this->remove_all_children();
        return;
    }

    if (strcmp(_text.c_str(), text) == 0) {
        return;
    } else {
        this->remove_all_children();
    }

    this->update_text(text);
}

void bmfont::update_text(const char* new_text)
{
    uint32_t codepoint = 0;
    uint32_t state = 0;
    size_t len = strlen(new_text);

    const uint8_t* p = (const uint8_t*)new_text;

    float cursor_x = 0.0f;
    float cursor_y = _font_info->common.lineHeight - _font_info->common.base;

    for (int i = 0; i < len; i ++) {
        if (util::utf8_decode(&state, &codepoint, p[i]) == UTF8_ACCEPT) {
             this->append_glyph(codepoint, &cursor_x, &cursor_y);
        }
    }

    _text = new_text;

     // TODO: add multiline support requires recaculate the size
    _size.width = cursor_x;
    _size.height = _font_info->common.lineHeight;
}

void bmfont::append_glyph(uint32_t codepoint, float *cursor_x, float *cursor_y)
{
    bmfont_character* charac = _font_info->_characters.at(codepoint);

    float pen_x = *cursor_x + charac->xoffset;
    float pen_y = *cursor_y /* + charac->yoffset */;

    rect r = rect::make_rect(charac->x, charac->y, charac->width, charac->height);

    sprite* c_sprite = new sprite();
    c_sprite->init(r, _texture);
    c_sprite->set_pos(pen_x, pen_y);
    c_sprite->set_anchor(0, 0);
    this->add_child(c_sprite);
    
    // TODO: multiline support, '\n' support, '\t' support
    *cursor_x = (*cursor_x) + charac->xadvance;
}

NS_S2D_END
