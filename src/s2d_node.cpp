//
//  Created by yiyang.tang on 2017/9/23.
//

#include "s2d_node.h"

NS_S2D

void node::init()
{
    _local_transform = affine_transform::mk_identity();
    _pos = {0, 0};

    _scale = {1.0, 1.0};
    _rotation = 0;
    _anchor = {0, 0};
    _size = {200*_scale.x, 400};
}

void node::update(float dt)
{
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

        // TODO: optimize this code
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotation);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);
    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);

    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
    affine_transform::inplace_concat(_local_transform, anchor_to);
}

void node::add_child(node* node)
{

}

void node::remove_child(node* node)
{

}

void node::remove_all_children()
{

}

void node::remove_from_parent()
{
    
}

NS_S2D_END
