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

#define USE_LODE_PNG

#ifdef USE_LODE_PNG
#include "lodepng.h"
#else
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

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

#ifdef USE_LODE_PNG

#if 1
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    lodepng::State state; //optionally customize this one

    unsigned char* data = nullptr;
    lodepng_decode32(&data, &width, &height, (const unsigned char*)f->_buffer, (size_t)f->_size);
#else

    unsigned int w = 1024;
    unsigned int h = 1024;

    unsigned int width = w / 4;
    unsigned int height = h / 4;

    uint8_t* data = new uint8_t[w * h];
    uint32_t *p = (uint32_t*)data;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            if (i < 128 && j < 128) {
                p[index] = 0x77000077;
            } else {
                p[index] = 0x00000000a22;
            }
        }
    }
#endif

#else
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
#endif

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    CHECK_GL_ERROR
#ifdef USE_LODE_PNG
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
CHECK_GL_ERROR
    _gl_handle = tex_id;
#ifdef USE_LODE_PNG
    _size.width = width;
    _size.height = height;
#else
    _size.width = x;
    _size.height = y;
#endif
}

NS_S2D_END
