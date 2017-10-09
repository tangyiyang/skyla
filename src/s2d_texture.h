/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#ifndef s2d_texture_h
#define s2d_texture_h

#include "s2d_common.h"
#include "s2d_file_system.h"
#include "s2d_gl_util.h"

NS_S2D

#define TEX_COORD_MAX ((1<<16)-1)

typedef uint64_t texture_id_t;

class texture : public ref_counter {
public:
    texture();
    ~texture();
    
    bool init(const char* file);
    inline void bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _gl_handle);
    }

    inline void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

public:
    texture_id_t    _id;
    file_entry* _file_entry;
    size        _size;
    GLint       _gl_handle;
 
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

#endif /* s2d_texture_h */
