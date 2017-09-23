//
//  Created by yiyang.tang on 2017/9/23.
//

#ifndef s2d_node_h
#define s2d_node_h

#include "s2d_common.h"

NS_S2D

class node {
public:
    virtual void init();
    virtual void update(float dt);

public:
    void add_child(node* child, uint32_t z_order = 0);
    void remove_child(node* child);
    void remove_all_children();
    void remove_from_parent();

public:
    inline uint32_t get_zorder()
    {
        return _z_order;
    }

protected:
    void sort();
    void update_srt();

protected:
    uint32_t             _z_order;
    // TODO: this could be a litter meomry wasting, consider a better way.
    uint32_t             _z_counter;
    uint64_t             _dirty_flags;
    node*                _parent;

    vec2                 _pos;
    vec2                 _scale;
    vec2                 _anchor;
    size                 _size;
    float                _rotation;
    affine_transform     _local_transform;

    std::vector<node*>   _children;
};

NS_S2D_END

#endif /* s2d_node_h */
