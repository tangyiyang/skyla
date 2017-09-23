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

texture::texture()
{
    _file_entry = nullptr;
    _size = {0, 0};
    _gl_handle = -1;
}

texture::~texture()
{
    SAFE_RELEASE(_file_entry);
}

void texture::init(const char* file)
{
    if (!file) {
        LOGE("init texture failed, file name is nil");
        return;
    }
    
    file_entry* f = util::load_file(file);
    f->retain();
    
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
        return;
    }
    
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    CHECK_GL_ERROR
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    CHECK_GL_ERROR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECK_GL_ERROR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    CHECK_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, 0);

    _gl_handle = tex_id;
    _size.width = x;
    _size.height = y;
}

NS_S2D_END
