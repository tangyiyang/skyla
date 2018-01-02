/*
 * Action class, handy for create GUI animations, and delayed events.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */
#ifndef s2d_action_h
#define s2d_action_h

#include "s2d_common.h"
#include "s2d_node.h"
#include "s2d_timer.h"

NS_S2D

class action;

class action_mgr {
public:
    void init();
    void add(action* act);
    void update(float dt);
    void remove(action* act);
    void remove(node* target);
    void shutdown();
    
public:
    std::vector<action*> _actions;
    std::map<node*, std::vector<action*> > _target_action_map;
};

class action {
public:
    virtual ~action() {}
    
    virtual void init(float duration);
    
    virtual void start(node* target) = 0;
    virtual bool update(float dt) = 0;
    virtual void stop();
    
public:
    float _duration;
    float _now;
    node* _target;
    bool _running;
};

class action_move : public action {
public:
    void init(float duration, float x, float y);
    void start(node* target);
    bool update(float dt);
    
private:
    vec2 _delta;
    vec2 _start_pos;
    vec2 _end_pos;
};

class action_scale: public action {
public:
    void init(float duration, float scale_x, float scale_y);
    
};

NS_S2D_END

#endif
