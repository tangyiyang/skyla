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

#include "s2d_texture.h"
#include "s2d_util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

NS_S2D

texture_id_t texture::_texture_id_counter = 0;
texture_cache::texture_cache()
{
    texture::_texture_id_counter = 0;
}

texture* texture_cache::load(const char* texture_file_name)
{
    std::map<std::string, texture*>::iterator found = _cache.find(texture_file_name);
    if (found != _cache.end()) {
        return found->second;
    } else {
        texture* tex = new texture();
        if (tex->init(texture_file_name)) {
            _cache[texture_file_name] = tex;
            return tex;
        } else {
            delete tex;
        }
    }

    // TODO: we may have a default texture embeded in engine,
    //       this would make the user easier to debug.
    return nullptr;
}

void texture_cache::unload(const char* texture_file_name)
{
    std::map<std::string, texture*>::iterator found = _cache.find(texture_file_name);
    if (found != _cache.end()) {
        texture* tex = found->second;
        tex->release();
        _cache.erase(found);
    } else {
        LOGE("texture: %s could not be found, may be double free?");
        S2D_ASSERT(false);
    }
}

texture::texture()
{
    _size = {0, 0};
    _gl_handle = -1;
}

texture::~texture()
{
}

bool texture::init(const char* file)
{
    if (!file) {
        LOGE("init texture failed, file name is nil");
        return false;
    }
    
    file_entry* f = util::load_file(file, false);

    int x = 0;
    int y = 0;
    int channels_in_file = 0;
    uint8_t* data = stbi_load_from_memory(f->_buffer,
                                          (int)f->_size,
                                          &x, &y,
                                          &channels_in_file,
                                          STBI_rgb_alpha);
    if (!data) {
        LOGE("unable to parse the texture file: %s", file);
        f->release();
        return false;
    }

    GLuint gl_handle;
    glGenTextures(1, &gl_handle);
    
    glBindTexture(GL_TEXTURE_2D, gl_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    _id = _texture_id_counter++;
    _gl_handle = gl_handle;
    _size.width = x;
    _size.height = y;

    free(data);
    f->release();

    return true;
}

NS_S2D_END
