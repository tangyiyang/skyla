/*
 * Action class, handy for create GUI animations, and delayed events.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */
#ifndef skyla_action_h
#define skyla_action_h

#include "skyla_common.h"
#include "game_objects/node.h"
#include "core/timer.h"

NS_SKYLA

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

    virtual void start(node* target);
    virtual bool update(float dt) = 0;
    virtual void stop();

protected:
    void update_step(float dt);

public:
    float _duration;
    float _now;
    float _step;
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

class action_scale_to: public action {
public:
    void init(float duration, float scale_x, float scale_y);
    void start(node* target);
    bool update(float dt);

private:
    vec2 _delta;
    vec2 _start_scale;
    vec2 _end_scale;
};

NS_SKYLA_END

#endif
