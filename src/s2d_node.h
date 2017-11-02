/*
 * Scene-Graph(Child-Parent) System 
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_node_h
#define s2d_node_h

#include "s2d_common.h"
#include "s2d_touch_handler.h"
#include "s2d_render_state.h"

#define DIRTY_TRANSFORM (1<<0)
#define DIRTY_SCALE     (1<<1)
#define DIRTY_ROTATION  (1<<2)
#define DIRTY_Z         (1<<3)

#define DIRTY_SRT (DIRTY_TRANSFORM | DIRTY_SCALE | DIRTY_ROTATION)
#define DIRTY_ALL (DIRTY_SRT | DIRTY_Z)

NS_S2D

class node {
    /*
     *  The scene-graph implemention class.
     */
public:
    virtual void init();
    virtual void update(float dt);
    virtual void draw(render_state* rs);
    virtual void hit_test(touch_handler* handler, touch_event* event);
    virtual void on_touch(touch_event* event);
    virtual rect bounds_in(node* space);

public:
    void add_child(node* child, uint32_t z_order = 0);
    void remove_child(node* child);
    void remove_all_children();
    void remove_from_parent();

    vec2 world_to_local(float world_x, float world_y);
    affine_transform transform_to(node* to);
    affine_transform local_to_world();

public:
    inline void set_visible(bool v)
    {
        _visible = v;
    }

    inline void set_pos(float x, float y)
    {
        _pos = {x, y};
    }

    inline void set_scale(float scale)
    {
        _scale.x = _scale.y = scale;
    }

    inline void set_scale(float sx, float sy)
    {
        _scale.x = sx;
        _scale.y = sy;
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

    inline void set_size(float width, float height)
    {
        _size = {width, height};
    }

    inline uint32_t get_zorder()
    {
        return _z_order;
    }

    inline void set_zorder(uint32_t z)
    {
        _z_order = z;
        _dirty_flags |= DIRTY_Z;
    }

protected:
    node* get_root();
    void sort();
    void update_srt();

protected:
    uint64_t             _id;
    uint32_t             _z_order;
    uint32_t             _z_counter;
    uint64_t             _dirty_flags;

    bool                 _visible;
    vec2                 _pos;
    vec2                 _scale;
    vec2                 _anchor;
    size                 _size;
    float                _rotation;
    affine_transform     _local_transform;
    node*                _parent;
    std::vector<node*>   _children;

    static uint64_t _node_id_counter;
};

NS_S2D_END

#endif /* s2d_node_h */
