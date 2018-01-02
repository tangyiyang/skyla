#include "s2d_context.h"
#include "s2d_util.h"
#include "s2d_node.h"
#include "s2d_action.h"

NS_S2D

uint64_t node::_node_id_counter = 0;

void node::init()
{
    _id = _node_id_counter++;
    _local_transform = affine_transform::mk_identity();
    _visible = true;
    _pos = {0, 0};
    _scale = {1.0, 1.0};
    _rotation = 0;
    _anchor = {0.5, 0.5};
    _size = {0, 0};
    _z_order = 0;
    _z_counter = 0;
    _parent = nullptr;
    _dirty_flags = DIRTY_ALL;
    _children.clear();
    _touch_callback = nullptr;
}

bool node::update(float dt)
{
    this->sort();
    this->update_srt();
    std::vector<node*>::iterator it = _children.begin();
    while(it != _children.end()) {
        if ((*it)->update(dt)) {
            (*it)->on_detach();
            delete *it;
            _children.erase(it);
        } else {
            ++it;
        }
    }

    return _dirty_flags & DIRTY_MARKED_RELEASE;
}

void node::draw(render_state* rs)
{
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        (*it)->draw(rs);
    }
}

void node::hit_test(touch_handler* handler, touch_event* event)
{
    node* parent = _parent ? _parent : this;
    affine_transform local_to_world_transform = this->transform_to(this->get_root());
    affine_transform world_to_local_transform = affine_transform::invert(local_to_world_transform);

    vec2 local = affine_transform::apply_transform(world_to_local_transform, event->_pos.x, event->_pos.y);

    rect bounds = this->bounds_in(_parent);
    bool contains = rect::contains(bounds, local.x, local.y);
    
    if (contains && (event->_phase == touch_event::TOUCH_BEGIN)) {
        handler->add_touch_node(this);
    }
    // visit the child recusively
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        (*it)->hit_test(handler, event);
    }
}


rect node::bounds_in(node* space)
{
    return {
        0,
        0,
        _size.width,
        _size.height
    };
}


void node::on_touch(touch_event* event)
{
    // TODO: add script
    if (_touch_callback) {
        _touch_callback(this, event);
    }
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
    child->_dirty_flags |= DIRTY_MARKED_RELEASE;
}

void node::remove_all_children()
{
    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {
        delete (*it);
    }
    _children.clear();
}

void node::on_detach()
{
    this->stop_all_actions();
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

void node::run_action(action* act)
{
    act->start(this);
}

void node::stop_all_actions()
{
    context::C()->_action_mgr->remove(this);
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

rect node::get_cascade_bounding()
{
    rect r = {
        0, 0, _size.width, _size.height
    };

    std::vector<node*>::iterator it = _children.begin();
    for (; it != _children.end(); ++it) {

    }

    return r;
}

rect node::get_bounding_box()
{
    
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
