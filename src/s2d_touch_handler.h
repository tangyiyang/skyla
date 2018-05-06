/*
 * Touch Handler class, it broad cast the touch events to each node.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_touch_handler_h
#define s2d_touch_handler_h

#include "s2d_common.h"
#include "s2d_util.h"

NS_S2D

class node;
struct touch_event;

typedef std::function<void(node*, touch_event*)> touch_callback_t;

struct touch_event {
    enum TOUCH_EVENT_PHASE {
        TOUCH_BEGIN = 0,
        TOUCH_MOVED,
        TOUCH_ENDED,
        TOUCH_CANCL,
    };

public:
    uint64_t          _id;
    vec2              _pos;
    TOUCH_EVENT_PHASE _phase;
};

class touch_handler {
public:
    static uint64_t touch_id()
    {
        return _touch_id_counter++;
    }

public:
    void init();

    void handle_touch_event(touch_event* event);

    inline void add_touch_node(node* n)
    {
        _touchable_nodes.push_back(n);
    }

private:
    void send_touch_events(touch_event* event);

private:
    std::vector<node*> _touchable_nodes;

private:
    static uint64_t _touch_id_counter;
};

NS_S2D_END

#endif
