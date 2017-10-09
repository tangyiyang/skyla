/****************************************************************************
 Copyright (c) Yiyang Tang

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "s2d_sprite.h"
#include "s2d_context.h"
#include "s2d_util.h"
#include "cJSON.h"

NS_S2D

void sprite_frame_cache::load(const char* json_atlas, const char* texture_file)
{
    texture* tex = util::load_texture(texture_file);

    file_entry* file = util::load_file(json_atlas, false);
    cJSON* root = cJSON_Parse((const char*)file->_buffer);
    cJSON* frames = cJSON_GetObjectItemCaseSensitive(root, "frames");
    if (frames) {
        cJSON* child = frames->child;
        while (child) {
            sprite_frame* f = new sprite_frame();
            const char* name = child->string;
            f->_name = name;

            cJSON* frame_obj = cJSON_GetObjectItemCaseSensitive(child, "frame");
            f->_frame.origin.x = cJSON_GetObjectItemCaseSensitive(frame_obj, "x")->valueint;
            f->_frame.origin.y = cJSON_GetObjectItemCaseSensitive(frame_obj, "y")->valueint;
            f->_frame.size.width = cJSON_GetObjectItemCaseSensitive(frame_obj, "w")->valueint;
            f->_frame.size.height = cJSON_GetObjectItemCaseSensitive(frame_obj, "h")->valueint;

            cJSON* size_obj = cJSON_GetObjectItemCaseSensitive(child, "sourceSize");
            f->_source_size.width = cJSON_GetObjectItemCaseSensitive(size_obj, "w")->valueint;
            f->_source_size.height = cJSON_GetObjectItemCaseSensitive(size_obj, "h")->valueint;

            cJSON* source_size_obj = cJSON_GetObjectItemCaseSensitive(child, "spriteSourceSize");
            f->_sprite_source_rect.origin.x = cJSON_GetObjectItemCaseSensitive(source_size_obj, "x")->valueint;
            f->_sprite_source_rect.origin.y = cJSON_GetObjectItemCaseSensitive(source_size_obj, "y")->valueint;
            f->_sprite_source_rect.size.width = cJSON_GetObjectItemCaseSensitive(source_size_obj, "w")->valueint;
            f->_sprite_source_rect.size.height = cJSON_GetObjectItemCaseSensitive(source_size_obj, "h")->valueint;

            f->_rotated = (cJSON_GetObjectItemCaseSensitive(child, "rotated")->type == cJSON_True);
            f->_trimmed = (cJSON_GetObjectItemCaseSensitive(child, "trimmed")->type == cJSON_True);

            f->_texture = tex;
            _cache[name] = f;

            child = child->next;
        }
    }

    cJSON_Delete(root);
    file->release();
}

sprite_frame* sprite_frame_cache::get(const char* name)
{
    return _cache[name];
}

void sprite_frame_cache::shutdown()
{
    std::map<std::string, sprite_frame*>::iterator it = _cache.begin();
    for (; it != _cache.end(); ++it) {
        it->second->release();
    }
    _cache.clear();
}

void sprite::init(const char* tex_file)
{
    S2D_ASSERT(tex_file);
    
    node::init();

    _texture = context::C()->_texture_cache->load(tex_file);
    S2D_ASSERT(_texture);
    setTextureCoord(nullptr, _texture);
    _size = _texture->_size;
}

void sprite::init(sprite_frame* frame)
{
    S2D_ASSERT(frame);
    node::init();

    _texture = frame->_texture;
    setTextureCoord(frame, _texture);
    _size = frame->_source_size;
}

void sprite::setTextureCoord(sprite_frame* frame, texture* tex)
{
    if (frame) {
        float tex_w = tex->_size.width;
        float tex_h = tex->_size.height;
        float x = frame->_frame.origin.x;
        float y = frame->_frame.origin.y;
        float ow = frame->_frame.size.width;
        float oh = frame->_frame.size.height;

        float w = ow;
        float h = oh;
        if (frame->_rotated) {
            std::swap(w, h);
        }

        uint16_t left = (uint16_t)(x/tex_w * (float)TEX_COORD_MAX);
        uint16_t right = (uint16_t)((x+w)/tex_w * (float)TEX_COORD_MAX);
        uint16_t bottom = (uint16_t)(y/tex_h * (float)TEX_COORD_MAX);
        uint16_t top = (uint16_t)((y+h)/tex_h * (float)TEX_COORD_MAX);

        // Notice:
        //  quad[0] -> bottom-left
        //  quad[1] -> bottom-right
        //  quad[2] -> top-left
        //  quad[3] -> top-right
        //  opengl texture is upside-down, so when we swap the bottom and the top.
        if (frame->_rotated) {
            _quad[0].uv.u = left;
            _quad[0].uv.v = bottom;
            _quad[1].uv.u = left;
            _quad[1].uv.v = top;
            _quad[2].uv.u = right;
            _quad[2].uv.v = bottom;
            _quad[3].uv.u = right;
            _quad[3].uv.v = top;
        } else {
            _quad[0].uv.u = left;
            _quad[0].uv.v = top;
            _quad[1].uv.u = right;
            _quad[1].uv.v = top;
            _quad[2].uv.u = left;
            _quad[2].uv.v = bottom;
            _quad[3].uv.u = right;
            _quad[3].uv.v = bottom;
        }

        _quad[0].pos.x = 0;
        _quad[0].pos.y = 0;
        _quad[0].color = 0xffffffff;

        _quad[1].pos.x = ow;
        _quad[1].pos.y = 0;
        _quad[1].color = 0xffffffff;

        _quad[2].pos.x = 0;
        _quad[2].pos.y = oh;
        _quad[2].color = 0xffffffff;

        _quad[3].pos.x = ow;
        _quad[3].pos.y = oh;
        _quad[3].color = 0xffffffff;
    } else {
        _quad[0].pos.x = 0;
        _quad[0].pos.y = 0;
        _quad[0].uv.u = 0;
        _quad[0].uv.v = TEX_COORD_MAX;
        _quad[0].color = 0xffffffff;

        _quad[1].pos.x = 0;
        _quad[1].pos.y = tex->_size.height;
        _quad[1].uv.u = 0;
        _quad[1].uv.v = 0;
        _quad[1].color = 0xffffffff;

        _quad[2].pos.x = tex->_size.width;
        _quad[2].pos.y = 0;
        _quad[2].uv.u = (1 <<16)-1;
        _quad[2].uv.v = TEX_COORD_MAX;
        _quad[2].color = 0xffffffff;

        _quad[3].pos.x = tex->_size.width;
        _quad[3].pos.y = tex->_size.height;
        _quad[3].uv.u = (1<<16)-1;
        _quad[3].uv.v = 0;
        _quad[3].color = 0xffffffff;
    }
}

void sprite::update(float dt)
{
    node::update(dt);
    this->draw();
}

void sprite::draw()
{
    affine_transform world = transform_to(this->get_root());
    context::C()->_sprite_renderer->draw(world, _quad, _texture);
}

rect sprite::bounds_in(node* space)
{
    float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
    if (space == this) {
        for (int i = 0; i < 4; ++i) {
            float x = _quad[i].pos.x;
            float y = _quad[i].pos.y;
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        }
    } else {
        affine_transform trans = this->transform_to(space);
        for (int i = 0; i < 4; ++i) {
            float x = _quad[i].pos.x;
            float y = _quad[i].pos.y;

            vec2 p = affine_transform::apply_transform(trans, x, y);

            minX = std::min(minX, p.x);
            maxX = std::max(maxX, p.x);
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
    }

    return {minX, minY, maxX - minX, maxY - minY};
}

NS_S2D_END
