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

#include "s2d_node.h"
#include "s2d_util.h"

NS_S2D

#define DIRTY_TRANSFORM (1<<0)
#define DIRTY_SCALE     (1<<1)
#define DIRTY_ROTATION  (1<<2)
#define DIRTY_Z         (1<<3)

#define DIRTY_SRT (DIRTY_TRANSFORM | DIRTY_SCALE | DIRTY_ROTATION)
#define DIRTY_ALL (DIRTY_SRT | DIRTY_Z)

uint64_t node::_node_id_counter = 0;
void node::init()
{
    _id = _node_id_counter++;
    _local_transform = affine_transform::mk_identity();
    _pos = {0, 0};
    _scale = {1.0, 1.0};
    _rotation = 0;
    _anchor = {0.5, 0.5};
    _size = {0, 0};
    _z_order = 0;
    _z_counter = 0;
    _parent = nullptr;
    _dirty_flags = DIRTY_ALL;
}

void node::update(float dt)
{
    this->sort();
    this->update_srt();
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        (*it)->update(dt);
    }
}

void node::hit_test(touch_handler* handler, touch_event* event)
{
    if (_id == 3) {
        // id == 3, sprite
        // bug: sprite not reponsed correctly.
        int i = 0;
    }
    node* parent = _parent ? _parent : this;
    affine_transform world_to_local_transform = affine_transform::invert(parent->transform_to(this->get_root()));

    vec2 local = affine_transform::apply_transform(world_to_local_transform, event->_pos.x, event->_pos.y);

    // TODO: we should have a virtual rect bounds() function????
    rect bounds = {
        0,
        0,
        _size.width,
        _size.height};
    bool contains = rect::contains(bounds, local.x, local.y);

    LOGD("id(%d), world = %.2f, %.2f, local = %.2f, %.2f, size = %.2f, %.2f, contains = %s", _id, event->_pos.x, event->_pos.y, local.x, local.y, _size.width, _size.height, contains?"true" : "false");

    if (contains && (event->_phase == touch_event::TOUCH_BEGIN)) {
        handler->add_touch_node(this);
    }

    // visit the child recusively
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        (*it)->hit_test(handler, event);
    }
}

void node::on_touch(touch_event* event)
{
    LOGD("on_touch id(%d), event = %d, p = %.2f, %,2f", this->_id, event->_phase, event->_pos.x, event->_pos.y);
}

void node::update_srt()
{
    _local_transform = affine_transform::mk_identity();

    float anchor_x = _size.width * _anchor.x;
    float anchor_y = _size.height * _anchor.y;

    //TODO: optimize this code
    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotation);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);

    affine_transform::inplace_concat(_local_transform, anchor_to);
    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
}

static bool comapre_z(node* a, node* b)
{
    return a->get_zorder() < b->get_zorder();
}

void node::sort()
{
    if (_dirty_flags & DIRTY_Z) {
        _dirty_flags &= (~DIRTY_Z);
        std::stable_sort(_children.begin(), _children.end(), comapre_z);
    }
}

void node::add_child(node* child, uint32_t z_order/*= 0*/)
{
    child->_z_counter++;

    child->_parent = this;
    _children.push_back(child);
}

void node::remove_child(node* child)
{
    _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
    delete child;
}

void node::remove_all_children()
{
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        delete (*it);
    }
    _children.clear();
}

void node::remove_from_parent()
{
    S2D_ASSERT(_parent != nullptr);
    _parent->remove_child(this);
}

vec2 node::world_to_local(float world_x, float world_y)
{
    affine_transform world_to_local = affine_transform::invert(local_to_world());
    return affine_transform::apply_transform(world_to_local, world_x, world_y);
}

affine_transform node::transform_to(node* to)
{
    // TODO: this code could be optimized.
    if (this == to) {
        return this->_local_transform;
    }

    affine_transform ret = affine_transform::mk_identity();

    node* from = this;
    while(from != to) {
        affine_transform::inplace_concat(ret, from->_local_transform);
        from = from->_parent;
    }

    return ret;
}

affine_transform node::local_to_world()
{
    return transform_to(this->get_root());
}

rect node::bounds()
{
    affine_transform world_to_local = transform_to(this->get_root());
    return {
        0,
        0,
        world_to_local.a * _size.width,
        world_to_local.d * _size.height
    };
}

node* node::get_root()
{
    node* parent = this;
    while(parent->_parent) {
        parent = parent->_parent;
    }
    return parent;
}

NS_S2D_END
