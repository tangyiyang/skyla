/*
 * Scene-Graph(Child-Parent) System
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_node_h
#define skyla_node_h

#include "skyla_common.h"
#include "core/touch_handler.h"
#include "renderer/render_state.h"

#define DIRTY_TRANSFORM      (1<<0)
#define DIRTY_SCALE          (1<<1)
#define DIRTY_ROTATION       (1<<2)
#define DIRTY_Z              (1<<3)
#define DIRTY_MARKED_RELEASE (1<<31)

#define DIRTY_SRT (DIRTY_TRANSFORM | DIRTY_SCALE | DIRTY_ROTATION)
#define DIRTY_ALL (DIRTY_SRT | DIRTY_Z)

NS_SKYLA

class action;
class node;

typedef std::function<void(node*, float)> update_callback_t;

class node {
    /*
     *  The scene-graph implemention class.
     */
public:
    node() {}
    virtual ~node() {}
    virtual void init();
    virtual bool update(float dt);
    virtual void draw(render_state* rs);
    virtual void hit_test(touch_handler* handler, touch_event* event);
    virtual void on_touch(touch_event* event);
    virtual rect local_bounds();

public:
    void                      add_child(node* child, uint32_t z_order = 0);
    const std::vector<node*>& get_children();
    void                      remove_child(node* child);
    void                      remove_all_children();
    void                      remove_from_parent();
    void                      on_detach();

    vec2             world_to_local(float world_x, float world_y);
    vec2             local_to_world();
    affine_transform transform_to(node* to);
    affine_transform local_to_world_transform();
    rect             get_bounding_box();

public:
    void run_action(action* act);
    void stop_all_actions();

public:
    inline uint64_t get_id()
    {
        return _id;
    }

    inline const vec2& get_pos()
    {
        return _pos;
    }

    inline const vec2& get_scale()
    {
        return _scale;
    }

    inline void set_visible(bool v)
    {
        _visible = v;
    }

    inline void set_swallow_touches(bool swallow)
    {
        _swallow_touches = swallow;
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

    inline const size& get_size()
    {
        return _size;
    }

    inline void set_size(float width, float height)
    {
        _size = {width, height};
    }

    inline uint32_t get_zorder()
    {
        return _z_order;
    }

    inline uint32_t get_zcounter()
    {
        return _z_counter;
    }

    inline bool is_swallow_touches()
    {
        return _swallow_touches;
    }

    inline bool is_touch_enabled()
    {
        return _touch_callback != nullptr;
    }

    inline void set_zorder(uint32_t z)
    {
        _z_order = z;
        _dirty_flags |= DIRTY_Z;
    }

    inline void set_touch_callback(touch_callback_t callback)
    {
        _touch_callback = callback;
    }

    inline void set_update_callback(update_callback_t callback)
    {
        _update_callback = callback;
    }

    inline std::string to_string()
    {
        char buf[256];
        snprintf(buf, 256, "{id = %llu, type = %s, _pos = {%.2f, %.2f}, _size = {%.2f, %.2f}}",
                 _id, typeid(this).name(), _pos.x, _pos.y, _size.width, _size.height);
        return std::string(buf);
    }

    inline const affine_transform& world_transform()
    {
        return _world_transform;
    }

protected:
    node* get_root();
    void sort();
    void update_srt();

protected:
    uint64_t             _id;
    uint32_t             _z_order;
    uint32_t             _z_counter;
    uint32_t             _dirty_flags;

    bool                 _visible;
    bool                 _swallow_touches;
    vec2                 _pos;
    vec2                 _scale;
    vec2                 _anchor;
    size                 _size;
    float                _rotation;
    affine_transform     _local_transform;
    affine_transform     _world_transform;
    node*                _parent;
    std::vector<node*>   _children;
    touch_callback_t     _touch_callback;
    update_callback_t    _update_callback;

    static uint64_t      _node_id_counter;
};

NS_SKYLA_END

#endif
