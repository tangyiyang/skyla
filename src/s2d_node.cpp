//
//  Created by yiyang.tang on 2017/9/23.
//

#include "s2d_node.h"
#include "s2d_util.h"

NS_S2D

#define DIRTY_TRANSFORM (1<<0)
#define DIRTY_SCALE     (1<<1)
#define DIRTY_ROTATION  (1<<2)
#define DIRTY_Z         (1<<3)

#define DIRTY_SRT (DIRTY_TRANSFORM | DIRTY_SCALE | DIRTY_ROTATION)
#define DIRTY_ALL (DIRTY_SRT | DIRTY_Z)

void node::init()
{
    _local_transform = affine_transform::mk_identity();
    _pos = {0, 0};

    _scale = {1.0, 1.0};
    _rotation = 0;
    _anchor = {0, 0};
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

void node::update_srt()
{
    _local_transform = affine_transform::mk_identity();

    float anchor_x = _size.width * _anchor.x;
    float anchor_y = _size.height * _anchor.y;

    //TODO: optimize this code
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotation);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);
    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);

    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
    affine_transform::inplace_concat(_local_transform, anchor_to);
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

NS_S2D_END
