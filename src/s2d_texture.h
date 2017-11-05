/*
 * Texure loading class.
 * Support formats: PNG
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_texture_h
#define s2d_texture_h

#include "s2d_common.h"
#include "s2d_file_system.h"

NS_S2D

#define TEX_COORD_MAX ((1<<16)-1)
#define UINT16_TEX_COORD(len, dim) ((uint16_t)((float)len/(float)dim)*(float)TEX_COORD_MAX)


typedef uint64_t texture_id_t;

class texture : public ref_counter {
public:
    texture();
    virtual ~texture();
    
    bool init(const char* file);
    inline void bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _name);
    }

    inline void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

public:
    texture_id_t    _id;
    size            _size;
    GLuint          _name;
    bool            _upside_down; /* if a texture is upside-down, render-texture is false.*/
    
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

NS_S2D_END

#endif
