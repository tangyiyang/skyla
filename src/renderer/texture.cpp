#include "renderer/texture.h"
#include "core/util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

NS_SKYLA

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
        SKYLA_ASSERT(false);
    }
}

texture::texture()
{
    _id   = -1;
    _size = {0, 0};
    _name = 0;
    _upside_down = true;
    _premultiply_alpha = true;
}

texture::~texture()
{
    if (_name) {
        glDeleteTextures(1, &_name);
    }
}

bool texture::init(const char* file)
{
    if (!file) {
        LOGE("init texture failed, file name is nil");
        return false;
    }

    file_entry* f = util::load_file(file, false);

    if (f) {
        this->init(f->_buffer, (int)f->_size);
        f->release();
        _file = file;
        return true;
    } else {
        LOGE("file: %s, could not be located.", file);
        return false;
    }
}

bool texture::init(uint8_t* raw_data, size_t len)
{
    int x = 0;
    int y = 0;
    int channels_in_file = 0;
    uint8_t* data = stbi_load_from_memory(raw_data,
                                          len,
                                          &x, &y,
                                          &channels_in_file,
                                          STBI_rgb_alpha);
    if (!data) {
        LOGE("unable to parse texture data.");
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

    free(data);

    _id = _texture_id_counter++;
    _name = gl_handle;
    _size.width = x;
    _size.height = y;

    return true;
}

NS_SKYLA_END
