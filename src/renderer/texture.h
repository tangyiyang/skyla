/*
 * Texure loading class.
 * Support formats: PNG
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_texture_h
#define skyla_texture_h

#include "skyla_common.h"
#include "platform/file_system.h"

NS_SKYLA

typedef uint64_t texture_id_t;

class texture : public ref_counter {
public:
    texture();
    virtual ~texture();

    bool init(const char* file);
    bool init(uint8_t* raw_data, size_t len);

    inline void bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _name);
    }

    inline void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    inline const std::string& get_file_path()
    {
        return _file;
    }

public:
    texture_id_t    _id;
    size            _size;
    GLuint          _name;
    bool            _upside_down; /* if a texture is upside-down, render-texture is true.*/
    bool            _premultiply_alpha;
    std::string     _file; /* file name of texture. Empty string if texture is initialized from raw data*/

    static texture_id_t _texture_id_counter;
};

class texture_cache {
public:
    texture_cache();

    texture* load(const char* texture_file_name);
    void unload(const char* texture_file_name);

private:
    std::map<std::string, texture*> _cache;
};

NS_SKYLA_END

#endif
