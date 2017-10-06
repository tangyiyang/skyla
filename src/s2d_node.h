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

#ifndef s2d_node_h
#define s2d_node_h

#include "s2d_common.h"
#include "s2d_touch_handler.h"

NS_S2D

class node {
    /*
     *  The scene-graph and the related model-view transformation.
     */
public:
    virtual void init();
    virtual void update(float dt);
    virtual void hit_test(touch_handler* handler, touch_event* event);
    virtual void on_touch(touch_event* event);

public:
    void add_child(node* child, uint32_t z_order = 0);
    void remove_child(node* child);
    void remove_all_children();
    void remove_from_parent();

    vec2 world_to_local(float world_x, float world_y);
    affine_transform transform_to(node* to);
    affine_transform local_to_world();

public:
    inline void set_pos(float x, float y)
    {
        _pos = {x, y};
    }

    inline void set_scale(float scale)
    {
        _scale.x = _scale.y = scale;
        _size.width = _content_size.width * scale;
        _size.height = _content_size.height * scale;
    }

    inline void set_scale(float sx, float sy)
    {
        _scale.x = sx;
        _scale.y = sy;
        _size.width = _content_size.width * sx;
        _size.height = _content_size.height * sy;
    }

    inline void set_anchor(float ax, float ay)
    {
        _anchor.x = ax;
        _anchor.y = ay;
    }

    inline void set_rotation(float rotation)
    {
        _rotation = rotation;
    }

    inline void set_content_size(float width, float height)
    {
        _content_size = _size = {width, height};
    }

    inline uint32_t get_zorder()
    {
        return _z_order;
    }

protected:
    node* get_root();
    void sort();
    void update_srt();
    bool contains(float local_x, float local_y);

protected:
    // members for the scene-graph.
    uint32_t             _z_order;
    uint32_t             _z_counter;
    uint64_t             _dirty_flags;

    vec2                 _pos;
    vec2                 _scale;
    vec2                 _anchor;
    size                 _size;
    size                 _content_size;
    float                _rotation;
    affine_transform     _local_transform;
    node*                _parent;
    std::vector<node*>   _children;
};

NS_S2D_END

#endif /* s2d_node_h */
