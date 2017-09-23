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
    void add_child(node* node);
    void remove_child(node* node);
    void remove_all_children();
    void remove_from_parent();

protected:
    void update_srt();

protected:
    uint32_t _z_order;

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
