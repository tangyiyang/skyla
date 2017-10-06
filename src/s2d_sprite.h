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

#ifndef s2d_sprite_h
#define s2d_sprite_h

#include "s2d_node.h"
#include "s2d_texture.h"
#include "s2d_program.h"
#include "s2d_sprite_renderer.h"

NS_S2D

class sprite_frame : public ref_counter {
public:
    rect _frame;
    rect _sprite_source_rect;
    size _source_size;
    bool _rotated;
    bool _trimmed;
    std::string _name;
    texture* _texture;
};

class sprite_frame_cache {
public:
    void load(const char* atlas, const char* texture_file);
    sprite_frame* get(const char* name);

    void shutdown();
private:
    std::map<std::string, sprite_frame*> _cache;
};

class sprite: public node {
public:
    void init(const char* tex_file);
    void init(sprite_frame* frame);
    void update(float dt) override;

protected:
    void draw();
    void update_srt();

protected:
    pos_tex_color_vertex _quad[4];
    texture*             _texture;
};

NS_S2D_END

#endif /* s2d_sprite_h */
